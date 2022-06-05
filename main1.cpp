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

Queue *q1 = new Queue();
Queue *q2 = new Queue();
Queue *q3 = new Queue();

/* ------------------Active Object Class -------------------------*/

class active_object
{

public:
    Queue *q;
    pthread_t *thread;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    void *(*start_routine)(void *);
    void *(*end_rountine)(void *);
    void *(*runThread1)(void *);
    int new_fd;

    active_object()
    {
        cout << "inside init active object" << endl;
        // q=new Queue();
        // q->createQ();
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);
    }

    ~active_object()
    {
        delete q;
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond);
    }

    active_object *newAO(Queue *q, void *(*start_routine)(void *), void *(*end_rountine)(void *))
    {
        active_object *ao = new active_object();
        ao->q = q;
        ao->start_routine = start_routine;
        ao->end_rountine = end_rountine;
        ao->new_fd = new_fd;
        ao->thread = new pthread_t;
        // send to run function
        cout << "before create thread" << endl;
        pthread_create((ao->thread), NULL, runThread, &ao);

        return ao;
    };

    // destroy active object
    void destroyAO(active_object *ao)
    {
        pthread_mutex_lock(&lock);
        pthread_cancel(*ao->thread);
        pthread_mutex_unlock(&lock);
        delete ao;
    };
    static void *runThread(void *arg)
    {
        cout << "inside run" << endl;
        // create active object
        //active_object *obj = new active_object();
        active_object *obj = (active_object*)arg;
        //*obj = *(active_object *)arg;
        void *data;
        while (1)
        {
            cout << "inside while" << endl;
            // data= obj->q->deQ(obj->q);
            data = obj->q->deQ();

            void *now_routine = obj->start_routine(data);
            void *ans = obj->end_rountine(now_routine);
        }
    }
};

/* ------------------Creata Pipeline -------------------------*/
typedef struct pipeline
{
    active_object *func1;
    active_object *func2;
    active_object *func3;
    // active_object *func4;
} pipeline;

/* ------------------Server -------------------------*/

// this function push the msg from the client to the queue
void *input_func1(char *temp)
{
    cout << "inside input_func1" << endl;
    q1->enQ(temp, q1);
    return temp;
}
void *output_func1(void *temp)
{
    cout << "inside output_func1" << endl;
    q2->enQ(temp, q2);
    return temp;
}
void *output_func2(void *temp)
{
    cout << "inside output_func2" << endl;
    
    q3->enQ(temp, q3);
    return temp;
}
void *output_func3(void *temp)
{
    cout << "inside output_func3" << endl;
    //send temp back to the client 
    char *tempp = (char *)temp;
    int sockfd= q3->sockfd;/////////@@@@ check where im saving the sock number of the client 
    int check = send(sockfd, tempp, 2048, 0);
    if (check == -1)
    {
        perror("send");
        exit(1);
    }
    return temp;
    }

void *func1(void *data)
{
    cout << "inside func1" << endl;
    char *ans = "";
    char *data1 = (char *)data;
    for (int i = 0; i < strlen(data1); i++)
    {
        if (data1[i] >= 'a' && data1[i] <= 'z')
        {
            ans += (data1[i] - 96) % 26 + 97;
        }
        else if (data1[i] >= 'A' && data1[i] <= 'Z')
        {
            ans += (data1[i] - 64) % 26 + 65;
        }
    }
    return (void *)ans;
}

// change upper case to lower case
void *func2(void *data)
{
    cout << "inside func2" << endl;
    char *ans = "";
    char *data1 = (char *)data;
    for (int i = 0; i < strlen(data1); i++)
    {
        if (data1[i] >= 'a' && data1[i] <= 'z')
        {
            ans += (data1[i] - 32);
        }
    }
    return (void *)ans;
}
void* func3(void *data){
    cout<<"inside func3"<<endl;
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

    // int sockfd= malloc(sizeof(((int*)ptr)));
    int sockfd = *((int *)ptr);
    // revc message from top of the queue

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
        printf("Received: %s\n", recv_msg);
        data = recv_msg;
        cout << "data is " << data << endl;
        // push the data to the queue
        input_func1(data);
    }

    // enqueue the message to the bottom of the queueu
}
// Caesar cipher

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

        // Each Client connect to the Server woth new thread
        pthread_t thread;
        pthread_create(&thread, NULL, get_function, (void *)&new_fd);
    }
    return 0;
}

int main()
{

    // create the queue
    q1->createQ();
    cout << "after cerate queue1" << endl;
    q2->createQ();
    cout << "after cerate queue2" << endl;
    q3->createQ();
    cout << "after cerate queue3" << endl;

    // create the active object
    active_object *ao = new active_object();
    active_object *ao2 = new active_object();
    ;
    active_object *ao3 = new active_object();
    cout << "after create active object" << endl;
    ao->newAO(q1, func1, output_func1);
    cout << "after create ao" << endl;
    ao2->newAO(q2, func2, output_func2);
    cout << "after create ao2" << endl;
    ao3->newAO(q3, func3, output_func3);
    cout << "after create ao3" << endl;

    // ao->q = q1;
    // ao2->q = q2;
    // ao3->q = q3;

    // /crete pipeline
    pipeline *p = new pipeline();
    p->func1 = ao;
    p->func2 = ao2;
    p->func3 = ao3;
    cout << "finish init" << endl;
    // create the server
    create_server();
}
