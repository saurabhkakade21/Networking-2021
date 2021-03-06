/************************************************************************
 * libraries
 ************************************************************************/
// should always be there ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h> //strlen

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>


/************************************************************************
 * function prototype declarations
 ************************************************************************/
void handle_client(int client_socket);

/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define SERVER_ADDR "129.6.15.28" // NIST ip address
#define PORT 13              // port the server will listen on

// I cannot let go of the old-fashioned way :) - for readability ...
#define FALSE false
#define TRUE !false

// number of pending connections in the connection queue
#define NUM_CONNECTIONS 1
