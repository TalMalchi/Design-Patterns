#include <iostream>
#include <string>
#include <pthread.h>
//

class Reactor{
    private:
    int id;
    void *(*callback)(void *);
    pthread_t thread;

    public:
       Reactor()= default;
       ~Reactor()= default; 
       void InstallHandler(int *file_num, void *(*callback)(void *)){
           this->id=*file_num;
            this->callback=callback;
            pthread_create(&(this->thread), NULL, callback, (void*)file_num);

       }
        void removeHandler(Reactor *reactor, int file_num){
            reactor->id=-1;
            reactor->callback= NULL;
            pthread_join(reactor->thread,NULL);
        }
        friend void* newReactor() {
        Reactor *new_one = new Reactor();
        return new_one;
    }
};

