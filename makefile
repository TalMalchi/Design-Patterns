all: main1

# client: client.o
# 	g++ -o client client.o 

main1: main1.o
	g++ -o main1 main1.o -lpthread

singelton: singelton.o
	g++ -o singelton singelton.o -lpthread

guard: guard.o
	g++ -o guard guard.o

guard.o: guard.cpp 
	g++ -c guard.cpp

singelton.o: singelton.cpp
	g++ -c singelton.cpp -lpthread

main1.o: main1.cpp
	g++ -c main1.cpp -lpthread 


.PHONY: clean main1


clean:
	rm -f *.o main1 singelton guard all