#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;
#define PORT 9034
int connected = 0;
int sockett = -1;

void *recvFunction(void *arg)
{
    char buff[1024] = {0};
    connected = 1;
    int bytes = 0;
    while ((bytes = recv(sockett, buff, 1024, 0)) != -1)
    {
        if (!bytes)
        {
            connected = 0;
            break;
        }
        cout<< "Received: " << buff << endl;
        if (!strcmp(buff, "EXIT"))
        {
            connected = 0;
            break;
        }
        bzero(buff, 1024);
    }
    return NULL;
}

void *sendFunction(void *arg)
{
    char input[1024] = {0};
    while (connected != 0)
    {

        cin>>input;
        cout<<"Cilent input: "<<input<<endl;
        if (strncmp(input,"EXIT",4) == 0)
        {
            send(sockett,"EXIT",4,0);
            connected = 0;
            break;
        }
        if (send(sockett, input, strlen(input) + 1, 0) == -1)
        {
            perror("send");
        }
        bzero(input, 1024);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    cout<< "New client connected" << endl;
    // create socket
    sockett = socket(AF_INET, SOCK_STREAM, 0);
    if (sockett == -1)
    {
        perror("socket");
        return -1;
    }
    //signal(SIGPIPE, SIG_IGN);

    
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    // initialize all fields of clientAddress 
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(PORT); // network order

    // connect to a server

    if (connect(sockett, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        
        cout<<"listen failed"<<endl;
        close(sockett);
        return -1;
    }

    pthread_t threadsPair[2];
    connected = 1;

    pthread_create(&threadsPair[1], NULL, sendFunction, NULL);
    pthread_create(&threadsPair[0], NULL, recvFunction, NULL);
    

    pthread_join(threadsPair[1], NULL);
    pthread_kill(threadsPair[1], 0);

    close(sockett);
    
    cout<< "Client disconnected" << endl;
    return 0;
}