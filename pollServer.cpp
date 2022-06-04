/*
** pollserver.c -- a cheezy multiperson chat server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include "Reactor.hpp"
using namespace std;
#define PORT "9034"   // Port we're listening on
void *newReactor();

// ReactorPtr create_reactor() {
//     ReactorPtr reactor =(ReactorPtr)malloc(sizeof(Reactor));
//     reactor->id = 0;
//     reactor->callback = NULL;
//     reactor->thread = 0;
//     return reactor;
// }
// // this function is called by the reactor thread
// void InstallHandler(ReactorPtr reactor, int file_num, void *(*callback)(void *)) {
//     reactor->id = file_num;
//     reactor->callback = callback;
//     ReactorManagerPtr manager = (ReactorManagerPtr)malloc(sizeof(ReactorManager));
//     manager->reactor_count = file_num;
//     manager->reactorPointer = reactor;
//     pthread_create(&reactor->thread, NULL, callback, reactor);
// }

// // This function is responsible for removing the handler from the reactor.
// void removeHandler(ReactorPtr reactor, int file_num) {
//     pthread_join(reactor->thread, NULL); // maybe change  it pthread join
//     reactor->id = -1;
//     reactor->callback = NULL;
    
// }
struct pollfd *pfds;
int listennum;
char buf[1024];
int fd_count;
// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void)
{
    int listener;     // Listening socket descriptor
    int yes=1;        // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { 
            continue;
        }
        
        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    // If we don't have room, add more space in the pfds array
    if (*fd_count == *fd_size) {
        *fd_size *= 2; // Double it

        *pfds = (struct pollfd *)realloc(*pfds, *fd_size * sizeof(struct pollfd));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN; // Check ready-to-read

    (*fd_count)++;
}

// Remove an index from the set
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*fd_count-1];

    (*fd_count)--;
}

// void* Reactor1(){
//     Reactor* reactor1 = create_reactor();
//     return reactor1;

// }
void *ThreadFunc(void *arg)
{

    int* temp = (int*)arg;
    int new_fd = *temp;
    while (1)
    {
        int bytes = recv(new_fd, buf, sizeof(buf), 0);
        if (bytes <= 0)
        {
            cout<<" Client disconnected" << endl;
            close(new_fd);
            return NULL;
        }
        else
        {
            for (int i = 0; i < fd_count + 1; i++)
            {
                
                int client_fd = pfds[i].fd;
        
                if (client_fd != listennum && client_fd != new_fd)
                {
                    send(client_fd, buf, bytes, 0);
                }
            }
            bzero(buf, 1024);
        }
    }
    //return NULL;
}

// Main
int main(void)
{
    cout<<"starting server.."<<endl;
  

    int newfd;        // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256];    // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    fd_count = 0;
    int fd_size = 5;
    pfds = (pollfd *)malloc(sizeof *pfds *fd_size);

    // Set up and get a listening socket
    listennum = get_listener_socket();

    if (listennum == -1) {
        fprintf(stderr, "error getting listening socket\n");
        exit(1);
    }

    // Add the listener to set
    pfds[0].fd = listennum;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    fd_count = 1; // For the listener

    // Main loop
    for(;;) {
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for(int i = 0; i < fd_count; i++) {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN) { // We got one!!

                if (pfds[i].fd == listennum) {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listennum,
                        (struct sockaddr *)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

                        printf("pollserver: new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                            //create new reactor
                            Reactor *reactor = (Reactor*)newReactor();
                            reactor->InstallHandler(&newfd,&ThreadFunc );

                    }
                }  // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!
    
    return 0;
}