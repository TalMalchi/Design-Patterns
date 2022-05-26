#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <string.h>



template <typename T>
class Queue {

 protected:
    struct node
      {
        T data;       // variable which holds the data
        node *next;   // variable which holds the address of the next node
      };
      node *head;           // variable which holds the address of the head node
      pthread_mutex_t lock; // variable which holds the lock

    public:
        Queue(){
            head = NULL;
    
        }
        ~Queue(){
            node *temp;
            while(head != NULL){
                temp = head;
                head = head->next;
                delete temp;
        }
        }
        void    createQ(){
            Queue();
        }
        void destroyQ(){
            ~Queue();
        }
        void enQ(T data){
            pthread_mutex_lock(&lock);
            if(head != NULL){
                
            node *temp = new node;
            temp->data = data;
            temp->next = head;
            }
            else{
                head = new node;
                head->data = data;
        }
        pthread_mutex_unlock(&lock);
        }
        void deQ(){
            pthread_mutex_lock(&lock);
            if(head != NULL){
                node *temp = head;
                head = head->next;
                delete temp;
            }
            pthread_mutex_unlock(&lock);
            throw "Queue is empty";
        }

        bool isEmpty(){
            return head == NULL;
        }


//create active object
class activeObject{

    private:
        pthread_t thread;
        pthread_mutex_t lock;
        pthread_cond_t cond;
        bool active;
        void (*func)(void *);
        void *arg;
        void *retval;
    void (*func_handle_before)();
    void (*func_handle_after)();


    public:
        activeObject(){
            active = false;
            pthread_mutex_init(&lock, NULL);
            pthread_cond_init(&cond, NULL);
        }
        ~activeObject(){
            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&cond);
        }
        void create(void (*func)(void *), void *arg){
            this->func = func;
            this->arg = arg;
            pthread_create(&thread, NULL, &activeObject::run, this);
        }
        void destroy(){
            pthread_cancel(thread);
            pthread_join(thread, NULL);
        }

         
        void set_func_handle_before(void (*func_handle_before)(void)){
            this->func_handle_before = func_handle_before;
        }
        void set_func_handle_after(void (*func_handle_after)(void)){
            this->func_handle_after = func_handle_after;
        }

        activeObject newAO(Queue q, void* handle_after, void* handle_before){
            activeObject *ao = new activeObject();
            ao->set_func_handle_after(handle_after);
            ao->set_func_handle_before(handle_before);
            ao->create(handle_after, q);
            return ao;

        }
        void deleteAO(){


        }
        // void wait(){
        //     pthread_mutex_lock(&lock);
        //     while(!active){
        //         pthread_cond_wait(&cond, &lock);
        //     }
        //     pthread_mutex_unlock(&lock);
        // }
        // void *join(){
        //     pthread_join(thread, &retval);
        //     return retval;
        // }
        // static void *run(void *arg){
        //     activeObject *obj = (activeObject *)arg;
        //     obj->func_handle_before();
        //     obj->func(obj->arg);
        //     obj->func_handle_after();
        //     pthread_mutex_lock(&obj->lock);
        //     obj->active = false;
        //     pthread_cond_signal(&obj->cond);
        //     pthread_mutex_unlock(&obj->lock);
        //     return NULL;
        // }
      
   

};
        

};
int main(){

//create queue
Queue<char> q;
q.createQ();


}
