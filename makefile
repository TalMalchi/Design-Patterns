all: main1 client singelton reactor guard clientR
 


clientR: clientReactor.o
	g++ -o clientR clientReactor.o -lpthread

client: client.o
	g++ -o client client.o 

reactor: pollServer.o
	g++ -o reactor pollServer.o -lpthread

main1: main1.o
	g++ -o main1 main1.o -lpthread

singelton: singelton.o
	g++ -o singelton singelton.o 

guard: guard.o
	g++ -o guard guard.o

pollServer.o: pollServer.cpp reactor.hpp
	g++ -c  pollServer.cpp reactor.hpp -lpthread

clientReactor.o: clientReactor.cpp 
	g++ -c  clientReactor.cpp 

client.o: client.cpp 
	g++ -c client.cpp

guard.o: guard.cpp 
	g++ -c guard.cpp

singelton.o: singelton.cpp
	g++ -c singelton.cpp 

main1.o: main1.cpp Queue.hpp
	g++ -c main1.cpp Queue.hpp -lpthread 


.PHONY: clean main1


clean:
	rm -f *.o main1 singelton guard all all1 reactor client clientR