#include <iostream>
#include <string>
#include <pthread.h>
//
typedef struct Reactor {
    int id;
    void *(*callback)(void *);
    pthread_t thread;

}Reactor, *ReactorPtr;

typedef struct ReactorManager {
    ReactorPtr reactorPointer;
    int reactor_count;
}ReactorManager, *ReactorManagerPtr;

ReactorPtr create_reactor();
void InstallHandler(ReactorPtr reactor, int file_num, void *(*callback)(void *));
void removeHandler(ReactorPtr reactor, int file_num);


