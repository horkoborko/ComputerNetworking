#include "computeserver.h"

// create global mutex lock
pthread_mutex_t lock;
/************************************************************************
 * MAIN
 ************************************************************************/

int main(int argc, char *argv[]) {
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket;
    pthread_t thread;                  // create thread id

    printf("Compute Server started\n");
    // sent when ,client disconnected
    signal(SIGPIPE, SIG_IGN);

    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(PORT);       // port to listen on

    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    // server loop
    while (true)
    {

        // accept connection to client
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1)
        {
            perror("Error accepting client");
        }else
        {
            // lock mutex
            pthread_mutex_lock(&lock);
            printf("\nAccepted client\n");
            //create new thread for every incoming client
            if(pthread_create( &thread, NULL, handle_client, &client_socket ) == -1)
            {
              perror("Error creating thread");
            }

            // detach tread
            pthread_detach(thread);


        }

    }
}


/************************************************************************
 * handle client
 ************************************************************************/

void * handle_client(void *arg)
{
    int client_socket = *( (int *) arg );
    // unlock mutex
    pthread_mutex_unlock(&lock);
    int firstNumber, operatorFlag;
    int secondNumber = -1;
    double result;

    // read ints from client
    read(client_socket, &firstNumber, sizeof(int));
    read(client_socket, &operatorFlag, sizeof(int));
    if (operatorFlag != 5)
    {
      read(client_socket, &secondNumber, sizeof(int));
    }

    // convert ints from client
    firstNumber = htonl(firstNumber);
    operatorFlag = htonl(operatorFlag);
    secondNumber = htonl(secondNumber);

    // see the numbers passed in
    printf("firstNumber: %d, operatorFlag: %d, secondNumber (if not provided it's weird): %d\n", firstNumber, operatorFlag, secondNumber);

    // do math
    result = doMath( firstNumber, operatorFlag, secondNumber );

    printf("result: %2.3f\n", result);

    // send result back to client
    send(client_socket, &result, sizeof(result), 0 );

    printf("Completed client\n");

    // cleanup
    if (close(client_socket) == -1)
    {
        perror("Error closing socket");
        pthread_exit(NULL);
    }
    else
    {
        printf("\nClosed socket to client, exit");
    }

    return 0;
}

double doMath(int firstNumber, int operatorFlag, int secondNumber)
{
   double result;
   if (operatorFlag == 0)
   {
      result = firstNumber + secondNumber;
   }
   else if(operatorFlag == 1)
   {
      result = firstNumber - secondNumber;
   }
   else if(operatorFlag == 2 )
   {
      result = firstNumber * secondNumber;
   }
   else if(operatorFlag == 3)
   {
      result = firstNumber / (double) secondNumber;
   }
   else if(operatorFlag == 4)
   {
      result = pow( (double)firstNumber, (double)secondNumber );
   }
   else
   {
      result = sqrt( (double)firstNumber );
   }

   return result;
}
