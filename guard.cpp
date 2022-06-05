
// implement gurad pattern

#include <pthread.h>
#include <iostream>
#include <mutex>

class gurad
{
    // one thread go into the guard and update global ptr
private:
    static gurad *instance;
    static int *global_ptr;
    static pthread_mutex_t lock;

public:
    gurad()
    {
        // lock
        pthread_mutex_lock(&lock);
    }
    ~gurad()
    {
        // unlock
        pthread_mutex_unlock(&lock);
    }

//update global ptr
    static void update_global_ptr(int *ptr)
    {
        global_ptr = ptr;
    }
};



/* Its not reccommaned to use this guard to protect strtok, Beacuse this gurad save just the last string
that has been read, and it may return incorrect string in case of multithreading */

int main(){
    return 0;
    
    
}