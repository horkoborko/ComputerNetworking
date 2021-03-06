/************************************************************************
 * libraries
 ************************************************************************/
// should always be there ...
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>

// read/write/close
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

/************************************************************************
 * preprocessor directives
 ************************************************************************/
#define SERVER_ADDR "time.nist.gov" // time server ip
#define PORT 13         // port the server will listen on
