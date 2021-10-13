/************************************************************************
 * libraries
 ************************************************************************/
// should always be there ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>

//For Threading
#include <pthread.h>

/************************************************************************
 * function prototype declarations
 ************************************************************************/
void handle_client(void *arg);
int ThreeAPlusOne(int input);
void *task_copy_arguments(void *args_in);

/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define SERVER_ADDR "127.0.0.1" // loopback ip address
#define PORT 23658              // port the server will listen on

// I cannot let go of the old-fashioned way :) - for readability ...
#define FALSE false
#define TRUE !false

// number of pending connections in the connection queue
#define NUM_CONNECTIONS 100

#define NUMBER_TASKS 1000
