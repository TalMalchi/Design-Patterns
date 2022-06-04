all: main  
all1: singelton reactor guard


# client: client.o
# 	g++ -o client client.o 
reactor: pollServer.o
	g++ -o reactor pollServer.o -lpthread

main: main.o
	g++ -o main main.o -lpthread

singelton: singelton.o
	g++ -o singelton singelton.o 

guard: guard.o
	g++ -o guard guard.o

pollServer.o: pollServer.cpp Reactor.hpp
	g++ -c  pollServer.cpp Reactor.hpp -lpthread

guard.o: guard.cpp 
	g++ -c guard.cpp

singelton.o: singelton.cpp
	g++ -c singelton.cpp 

main.o: main.cpp Queue.hpp
	g++ -c main.cpp Queue.hpp -lpthread 


.PHONY: clean main


clean:
	rm -f *.o main singelton guard all all1 reactor