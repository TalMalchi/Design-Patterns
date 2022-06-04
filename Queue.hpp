
#include <iostream>
using namespace std;
#include <pthread.h>
#include <mutex>
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

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
        head = NULL;
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);
        cout << "inside init queue" << endl;
    }

    Queue() {}
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

    void *deQ(Queue* q) // add the qeueue
    {
        cout << "inside deQ" << endl;
        //lock the queue
        cout<<"data:: "<< q->head->data << endl;
        pthread_mutex_lock(&(q->lock));
        
        cout << "after mutex lock" << endl;
        while (q->head == NULL)
        {
            cout << "inside deQ, before cond wait" << endl;
            pthread_cond_wait(&(q->cond), &(q->lock));
        }
        cout << "after while" << endl;
        void *data = q->head->data;
        cout << "data:: " << data << endl;
        node *temp = q->head;
        q->head = q->head->next;
        delete temp;
        pthread_mutex_unlock(&(q->lock));
        return data;
    }
};