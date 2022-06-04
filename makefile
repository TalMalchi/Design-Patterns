all: main  
all1: singelton reactor guard

# client: client.o
# 	g++ -o client client.o 
reactor: reactor.o
	g++ -o reactor reactor.o -lpthread

main: main.o
	g++ -o main main.o -lpthread

singelton: singelton.o
	g++ -o singelton singelton.o 

guard: guard.o
	g++ -o guard guard.o

reactor.o: Reactor.cpp Reactor.hpp
	g++ -c  Reactor.cpp Reactor.hpp -lpthread

guard.o: guard.cpp 
	g++ -c guard.cpp

singelton.o: singelton.cpp
	g++ -c singelton.cpp 

main.o: main.cpp Queue.hpp
	g++ -c main.cpp Queue.hpp -lpthread 


.PHONY: clean main


clean:
	rm -f *.o main singelton guard all all1