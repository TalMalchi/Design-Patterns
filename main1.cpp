#include <iostream>
#include <pthread.h>
#include <mutex>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "Queue.hpp"

#define PORT "3493" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

Queue *q1;
Queue *q2;
Queue *q3;

/* ------------------Active Object Class -------------------------*/

class active_object
{

public:
    Queue *q;
    pthread_t *thread;
    void *(*start_routine)(void *);
    void *(*end_rountine)(void *);
    void *(*runThread1)(void *);
    int new_fd;

    active_object()
    {
        q = new Queue();
        thread = new pthread_t;
        start_routine = NULL;
        end_rountine = NULL;
        runThread1 = NULL;
        new_fd = -1;
    }

    ~active_object()
    {
        delete q;
    }

    /* init new Active Object. Each active object has its own queue of tasks,
    and its own thread, that is responsible for executing the tasks. */

    active_object *newAO(Queue *q, void *(*start_routine)(void *), void *(*end_rountine)(void *))
    {
        this->q = q;
        this->start_routine = start_routine;
        this->end_rountine = end_rountine;
        this->new_fd = -1;
        this->thread = new pthread_t;
        pthread_create(this->thread, NULL, this->runThread, this);
        return this;
    };

    // destroy active object
    void destroyAO(active_object *ao)
    {
        pthread_cancel(*ao->thread);
        delete ao;
    };

    /* This function will run the thread of the active object, and will executr thw task in its own queue */
    static void *runThread(void *arg)
    {
        // cout << "inside run" << endl;
        active_object *obj = (active_object *)arg;
        while (1)
        {
            void *now_routine = obj->start_routine(obj->q->deQ());
            void *ans = obj->end_rountine(now_routine);
        }
    }
};

/* ------------------Server -------------------------*/

// enQ the message from the client to Q1- the queue of active object 1
void *input_func1(node *temp)
{
    q1->enQ(temp);
    return temp;
}
// enQ the new word to Q2- the queue of active object 2
void *output_func1(void *temp)
{
    q2->enQ(temp);
    return temp;
}
// enQ the new word to Q3- the queue of active object 3
void *output_func2(void *temp)
{
    q3->enQ(temp);
    return temp;
}
// send the new data to the client
void *output_func3(void *temp)
{
    node *temp1 = (node *)temp;
    // send temp back to the client
    if (strlen((char *)temp1->data) != 0)
    {
        int check = send(temp1->sockfd, temp1->data, strlen((char *)temp1->data), 0);
        if (check == -1)
        {
            perror("sockfd error");
            exit(1);
        }
        printf("Send back %s\n", (char *)(temp1->data));
    }

    return temp;
}

// Caesar cipher for the message
void *func1(void *data)
{
    node *temp = (node *)data;

    char *data1 = (char *)temp->data;
    for (int i = 0; i < strlen(data1); i++)
    {
        if (data1[i] >= 'a' && data1[i] <= 'z')
        {
            if (data1[i] == 'z')
            {
                data1[i] = 'a';
            }
            else
            {
                data1[i] = data1[i] + 1;
            }
        }
        else
        {
            if (data1[i] == 'Z')
            {
                data1[i] = 'A';
            }
            else
            {
                data1[i] = data1[i] + 1;
            }
        }
    }
    temp->data = data1;
    return temp;
}

// change upper case to lower case
void *func2(void *data)
{
    node *temp = (node *)data;
    char *data1 = (char *)temp->data;
    for (int i = 0; i < strlen(data1); i++)
    {
        if (data1[i] >= 'a' && data1[i] <= 'z')
        {
            data1[i] -= 32;
        }
        else
        {
            data1[i] += 32;
        }
    }
    temp->data = data1;
    return temp;
}
// return the new data
void *func3(void *data)
{
    return data;
}

void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}
// this function gtes a message from the client
void *get_function(void *ptr)
{

    int sockfd = *((int *)ptr);
    char recv_msg[1024];
    char *data;
    while (1)
    {

        memset(recv_msg, '\0', sizeof(recv_msg));
        int recv_len = recv(sockfd, recv_msg, sizeof(recv_msg), 0);
        if (recv_len == -1)
        {
            perror("recv");
            exit(1);
        }
        if (recv_len != 0)
        {

            printf("Received: %s\n", recv_msg);
            data = recv_msg;
            node *temp = new node;
            temp->data = new char[strlen(data) + 1];
            strcpy((char *)temp->data, data);
            temp->sockfd = sockfd;
            temp->next = NULL;

            // enQ the data to the queue
            input_func1(temp);
        }
    }
}

int create_server(void)
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;

    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (true)
    { // main accept() loop
        sin_size = sizeof their_addr;
        // get new connection for Client
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        // Each Client connect to the Server with new thread
        pthread_t thread;
        pthread_create(&thread, NULL, get_function, (void *)&new_fd);
    }
    return 0;
}

int main()
{

    // create the queue for each active object
    q1 = q1->createQ();
    q2 = q2->createQ();
    q3 = q3->createQ();

    /*  -------------------pipeline of active object-----------------------------
     Each active Object will change the data of the client, and pass it to the next on.
    ao- will change tha data by caeser chiper, add the new word to Q2 - the queue of ao2.
    a02- will change upper case letters to lowers, and the opposite, add the new word to Q3- the queue of ao3.
    ao3- will just return the data, and send the new work back to the client.   */

    active_object *ao = new active_object();
    active_object *ao2 = new active_object();
    active_object *ao3 = new active_object();
    ao = ao->newAO(q1, func1, output_func1);
    ao2 = ao2->newAO(q2, func2, output_func2);
    ao3 = ao3->newAO(q3, func3, output_func3);
    cout << "finish init" << endl;
    // run the server
    create_server();
}
