#include "computeserver.h"


/************************************************************************
 * MAIN
 ************************************************************************/

int main() {
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    struct sockaddr client_address;
    socklen_t client_address_length = sizeof(client_address);
    ssize_t bytes_received;

    char *client_hello = "Server says hi";
    int receivedArray[3];
    double result;


    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error creating socket\n");
        exit(EXIT_FAILURE);
    }

    // name the socket (making sure the correct network byte ordering is observed)
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = INADDR_ANY; // accept clients on any interface
    server_address.sin_port        = htons(PORT);       // port to listen on

    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0 ) {
        perror("Error binding socket\n");
        exit(EXIT_FAILURE);
    }


    // server loop
    while (true)
    {

      printf( "Computation started\n" );
      // receive int array from *any* client
      // client_addr will be filled in with addr info of where the data was received from
      if ((bytes_received = recvfrom(server_socket,
                                &receivedArray,
                                sizeof(receivedArray), MSG_WAITALL,
                                (struct sockaddr *) &client_address,
                                &client_address_length )) == -1)
      {
          perror("error when trying recvfrom\n");
      }

      printf( "Client equation: ");

      // convert ints from client
      receivedArray[0] = htonl(receivedArray[0]);
      receivedArray[1] = htonl(receivedArray[1]);
      receivedArray[2] = htonl(receivedArray[2]);

      if (receivedArray[1] == ADD)
      {
        printf( "%d + %d\n", receivedArray[0], receivedArray[2]);
     } else if (receivedArray[1] == SUBTRACT)
     {
        printf( "%d - %d\n", receivedArray[0], receivedArray[2]);
     } else if (receivedArray[1] == MULTIPLY)
     {
        printf( "%d x %d\n", receivedArray[0], receivedArray[2]);
     } else if (receivedArray[1] == DIVIDE)
     {
        printf( "%d / %d\n", receivedArray[0], receivedArray[2]);
     } else if (receivedArray[1] == RAISE)
     {
        printf( "%d ^ %d\n", receivedArray[0], receivedArray[2]);
     } else
     {
        printf( "sqrt of %d\n", receivedArray[0]);
     }

      result = handle_client( receivedArray );

      // send message back to client
      // data is contained in datagram
      // the client addr info is in client_address, which was filled in in recvfrom() call before
      if(sendto(server_socket, &result, sizeof(result), 0,
             (struct sockaddr *) &client_address,
             client_address_length) == -1)
      {
          perror("error when trying to sendto\n");
      }

      }

      return EXIT_SUCCESS;


}


/************************************************************************
 * handle client
 ************************************************************************/

double handle_client( int receivedArray[] )
{
    int firstNumber, operatorFlag;
    int secondNumber = -1;
    double result;

   firstNumber = receivedArray[0];
   operatorFlag = receivedArray[1];

    if (operatorFlag != SQRT)
    {
      secondNumber = receivedArray[2];
    }

    // do math
    result = doMath( firstNumber, operatorFlag, secondNumber );

    printf("result: %2.3f\n", result);

    return result;
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
