// #include <iostream>
// #include <pthread.h>


// //implemnet queue class
// template <typename T>
// class Queue {
// //heeeeey
//  protected:
//     struct node
//       {
//         T data;       // variable which holds the data
//         node *next;   // variable which holds the address of the next node
//       };
//       node *head;           // variable which holds the address of the head node
//       pthread_mutex_t lock; // variable which holds the lock

//     public:
//         Queue(){
//             head = NULL;
    
//         }
//         ~Queue(){
//             node *temp;
//             while(head != NULL){
//                 temp = head;
//                 head = head->next;
//                 delete temp;
//         }
//         }
//         void createQ(){
//             Queue();
//         }
//         void destroyQ(){
//             ~Queue();
//         }
//         void enQ(T data){
//             pthread_mutex_lock(&lock);
//             if(head != NULL){
                
//             node *temp = new node;
//             temp->data = data;
//             temp->next = head;
//             }
//             else{
//                 head = new node;
//                 head->data = data;
//         }
//         pthread_mutex_unlock(&lock);
//         }
//         void deQ(){
//             pthread_mutex_lock(&lock);
//             if(head != NULL){
//                 node *temp = head;
//                 head = head->next;
//                 delete temp;
//             }
//             pthread_mutex_unlock(&lock);
//             throw "Queue is empty";
//         }

//         bool isEmpty(){
//             return head == NULL;
//         }
// };