//implement singleton with mutex locking


#include <pthread.h>
#include <iostream>
#include <mutex>

using namespace std;

class singleton {

    // /private:
        

        
    public:
    static singleton *instance;
    static mutex mtx;
        singleton() {

        }
        ~singleton() {}

        static singleton* Instance() {

            if(instance == NULL) {
                mtx.lock();
                if(instance == NULL) {
                    instance = new singleton();
                }
                mtx.unlock();
            }
            return instance;
        }

        static void destroy() {
            if(instance != NULL) {
                mtx.lock();
                if(instance != NULL) {
                    delete instance;
                    instance = NULL;
                }
                mtx.unlock();
            }
        }
};

int main() {
    singleton *s = singleton::Instance();
    singleton::destroy();
    return 0;
    
    
}