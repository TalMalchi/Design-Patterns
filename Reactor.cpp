// #include <iostream>
// #include "Reactor.hpp"


// ReactorPtr create_reactor() {
//     ReactorPtr reactor =(ReactorPtr)malloc(sizeof(Reactor));
//     reactor->id = 0;
//     reactor->callback = NULL;
//     reactor->thread = 0;
//     return reactor;
// }
// // this function is called by the reactor thread
// void InstallHandler(ReactorPtr reactor, int file_num, void *(*callback)(void *)) {
//     reactor->id = file_num;
//     reactor->callback = callback;
//     ReactorManagerPtr manager = (ReactorManagerPtr)malloc(sizeof(ReactorManager));
//     manager->reactor_count = file_num;
//     manager->reactorPointer = reactor;
//     pthread_create(&reactor->thread, NULL, callback, reactor);
// }

// // This function is responsible for removing the handler from the reactor.
// void removeHandler(ReactorPtr reactor, int file_num) {
//     pthread_join(reactor->thread, NULL); // maybe change  it pthread join
//     reactor->id = -1;
//     reactor->callback = NULL;
    
// }

// // int main(){
// //     return 0;
// // }