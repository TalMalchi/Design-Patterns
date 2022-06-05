//implement singleton with mutex locking


#include <pthread.h>
#include <iostream>
#include <mutex>

using namespace std;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

//templates class singleton
template <class T>
class singleton
{
    private:
        static singleton *instance;
        T *ptr;
        singleton( T *ptr )
        {
            this->ptr = ptr;
        }

    public:
    //create new instance (singelton), and return the pointer to it
        static singleton *Instance( T *ptr )
        {
            if( instance == NULL )
            {
                pthread_mutex_lock(&mtx);
                if( instance == NULL )
                {
                    instance = new singleton( ptr );
                }
                pthread_mutex_unlock(&mtx);
            }
            return instance;
        }
        T *getPtr()
        {
            return ptr;
        }

        //destroy singleton
        static void Destroy()
        {
            pthread_mutex_lock(&mtx);
            if( instance != NULL )
            {
                delete instance;
                instance = NULL;
            }
            pthread_mutex_unlock(&mtx);
        }


};
//init the instance to NULL
template <typename T> singleton<T> *singleton<T>::instance = NULL;

int main(){
    ///create file descriptor with singleton for checking
    FILE *fp ;
    singleton<FILE> *check = singleton<FILE>::Instance( fp );
    singleton<FILE> *check1 = singleton<FILE>::Instance( fp );
    cout << check << endl;
    cout << check1 << endl;
    return 0;
}