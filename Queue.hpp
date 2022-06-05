
#include <iostream>
using namespace std;
#include <pthread.h>
#include <mutex>

struct node
{
    void *data;
    node *next;
    int sockfd;
};
node *head = NULL;

/* ------------------Queue Class -------------------------*/

class Queue
{

public:
    node *head;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    Queue *createQ()
    {
        Queue *q = new Queue();
        return q;
    }

    Queue()
    {
        this->head = NULL;
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);
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
    void *deQ() // add the qeueue
    {

        pthread_mutex_lock(&lock);
        while (this->head == NULL)
        {
            pthread_cond_wait(&(this->cond), &(this->lock));
        }
        node *temp = this->head;
        this->head = this->head->next != NULL ? this->head->next : NULL;
        pthread_mutex_unlock(&(lock));
        return temp;
    };
    
    // enqueue function to add data to the queue get data, queue, and sockfd
    void enQ(void *data)
    {
        // print data as string
        node *temp = (node *)data;
        pthread_mutex_lock(&(this->lock));
        if (this->head != NULL)
        {
            // node *temp = new node;
            node *curr = this->head;
            while (curr->next != NULL)
            {
                curr = curr->next;
            }
            curr->next = temp;
        }
        else
        {
            this->head = temp;
            this->head->data = temp->data;
            this->head->next = NULL;
            pthread_cond_signal(&(this->cond));
        }
        pthread_mutex_unlock(&(this->lock));
    }
};