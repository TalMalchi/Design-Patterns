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
template <typename T> singleton<T> *singleton<T>::instance = NULL;

int main(){
    ///create file descriptor with singleton
    FILE *fp ;
    singleton<FILE> *s = singleton<FILE>::Instance( fp );
    singleton<FILE> *s1 = singleton<FILE>::Instance( fp );
    cout << s << endl;
    cout << s1 << endl;
    return 0;
}