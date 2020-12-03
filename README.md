# Inter Terminals Chat

Unix inter terminal chat system developed for learning about threads, sockets and distributed systems.

## Development

This project has been developed with C++ language using *pthreads.h* and *sockets.h* libraries.

## Building and Running

As the name of the system suggests, the correct functioning of the program occurs with the execution of instances of the codes compiled in different command terminals of Unix Based systems. The server must be the first program to be executed and this must be done only once. After the server indicates which ports are available for connection, client programs must be run on different command terminals.

#### Building and Running server.cpp

To compile the server.cpp code, run the following command line:

`$ g++ -o server server.cpp -lpthread`

To execute the compiled program, the following command line must be executed:

`$ ./server <numberOfConnections>`

#### Building and Running client.cpp

To compile the client.cpp code, run the following command line:

`$ g++ -o client client.cpp -lpthread`

To execute the compiled program, the following command line must be executed:

`$ ./client <portNumber> <nickName>`