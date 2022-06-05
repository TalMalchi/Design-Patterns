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
       
       /* this function will get a pointer to the callback function and the id of the thread, 
       and will create a new thread that will handle the input of the file num */ 
       void InstallHandler(int *file_num, void *(*callback)(void *)){
           this->id=*file_num;
            this->callback=callback;
            pthread_create(&(this->thread), NULL, callback, (void*)file_num);

       }
         /* this function will released the reactor */
        void removeHandler(Reactor *reactor, int file_num){
            reactor->id=-1;
            reactor->callback= NULL;
            pthread_join(reactor->thread,NULL);
        }
        /*THis function Create new Reactor and return the pointer to it */
        friend void* newReactor() {
        Reactor *new_one = new Reactor();
        return new_one;
    }
};
