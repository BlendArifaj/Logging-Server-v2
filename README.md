# Logging-Server-v2
Creating a Logging Server using Sockets, Named Pipes (FIFO), Shared Memory and Shared Semaphores
# Usage
This program will allow clients and server to communicate using sockets, named pipes, shared memory and shared semaphore.

## Execute
To make program usable you have to make exe file, writing in this directorium: make all.

## Generate Documentation
To Generate the documentation for the functions you have to write, in this directprium: make dox.

## How to use program
After making the exe file, to execute the program you write: ./output/exe/exe -help (-h, --h or --help), and this will show the allowed arguments and their functionality.
After you make all, in output/exe will create two exe filed client and server. 
First you initiate the server using ./output/exe/server -f arg. After you initiate the server, it will create message queue, shared memory and semaphore.
When you execute ./output/exe/client -f arg1 -t arg2 you will initiate the client. Client will send a request on message queue to communicate with server. Server after handled the message queue will create a named pipe that will allow client to send request in server.
Client will send request using named pipe and server will handled that using conn_handler process. Conn handler will get the message on namep pipe and will try to attach shared memory to write it. After it write on shared memory, another proces will be invoked, record, and will read from shared memory and write on file.

## How to terminate program
To terminate the server use CTRL+C. After you press CTRL+C, sockets, shared memory and shared segment will be deleted, and also memory that was allocated will be released.
