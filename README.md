# Design-Patterns
In this  assigmment our main goal was to implement different types of design patterns such as:
* Reactor - The reactor design pattern is an event handling pattern for handling service requests delivered concurrently to a service handler by one or more inputs.
  The service handler then demultiplexes the incoming requests and dispatches them synchronously to the associated request handlers.

* Singelton -  The singleton pattern is a software design pattern that restricts the instantiation of a class to one "single" instance. This is useful when exactly one object is needed to coordinate actions across the system.

* Active Object - The active object design pattern decouples method execution from method invocation for objects that each reside in their own thread of control. The goal is to introduce concurrency, by using asynchronous method invocation and a scheduler for handling requests.  
We ask to create a pipeline of 3 active object. Each active Object will change the data of the message we got from client, and pass it to the next on.  
ao- will change tha data by caeser chiper, add the new word to Q2 - the queue of ao2.  
a02- will change upper case letters to lowers, and the opposite, add the new word to Q3- the queue of ao3.  
ao3- will just return the data, and send the new work back to the client.  

* Guard -  a guard is a boolean expression that must evaluate to true if the program execution is to continue in the branch in question.


In the Active object design patten we also implemented a save thread queue and and with this design pattern
we implemented a list of action such as :  
* Reading input from a server
* Using Caesar Cipher and transfer the input into it
* Transfer capital letters into small letters and the opposite
* And finally return to the the user all this data

## Main Goal of this project

The main goal of this project is a good and deep understanding of all the "Design Patterns" field,
At the beggigng it was a luttle bit difficulte to see implement parts  of this assigments
but after a good research we manage to complete this task.

# How to Run

In aim to compile all the file you need to write in your terminal `make all`.  
For questions 1-3 run from first terminal `./main1`, and from second terminal run `./client 127.0.0.1`   
For question 4 run from terminal `./guard`  
For question 5 run from terminal `./singelton`  
For question 6 run from first terminal `./reactor`, and from second terminal run `./clientR`   

In order to clean all the files you need to run `make clean`

A video which shows how to run the Active Object part:



https://user-images.githubusercontent.com/87694635/172058989-cbbf432b-621f-4c41-a745-9bd7a8dc9107.mp4


