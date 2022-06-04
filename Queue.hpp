
#include <iostream>
using namespace std;
#include <pthread.h>
#include <mutex>
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;

struct node
{
    void *data;
    node *next;
};
node *head = NULL;

/* ------------------Queue Class -------------------------*/

class Queue
{
    // protected:

public:
    node *head;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int sockfd;
    Queue *createQ()
    {
        Queue *q=new Queue();
        return q;
    }

    Queue() {
        head = NULL;
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);
        cout << "inside init queue" << endl;
    }
    void destroyQ()
    {
        node *temp;
        while (head != NULL)
        {
            temp = head;
            head = head->next;
            delete temp;
        }
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond);
    }
    void* deQ() // add the qeueue
    {
        //pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
        //pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
        cout << "inside deQ" << endl;
        pthread_mutex_lock(&lock);
        cout << "after mutex lock" << endl;
        while (head == NULL)
        {
            cout << "inside deQ, before cond wait" << endl;
            pthread_cond_wait(&(cond), &(lock));
        }
        cout << "after while" << endl;
        void *data = head->data;
        cout << "data:: " << data << endl;
        node *temp = head;
        head = head->next;
        delete temp;
        pthread_mutex_unlock(&(lock));
        return data;
    };
    // enqueue function to add data to the queue get data, queue, and sockfd
    void enQ(void *data, Queue *q)
    {
        cout << "inside enQ" << endl;
        pthread_mutex_lock(&(q->lock));
        if (q->head != NULL)
        {
            node *temp = new node;
            temp->data = data;
            temp->next = q->head;
            q->head = temp;
        }
        else
        {
            cout << "inside enQ else" << endl;
            q->head = new node;
            q->head->data = data;
            q->head->next = NULL;
        }
        // q->sockfd = sockfd;
        pthread_cond_signal(&(q->cond));
        pthread_mutex_unlock(&(q->lock));
        cout << "after enQ" << endl;
    }

    
};