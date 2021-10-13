#include "s_server.h"

/************************************************************************
 * MAIN
 ************************************************************************/

int main(int argc, char *argv[]) {
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket;

    printf("Server started\n");
    // sent when ,client disconnected
    signal(SIGPIPE, SIG_IGN);

    // create unnamed network socket for server to listen on
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket\n");
        exit(EXIT_FAILURE);
    }

    // name the socket (making sure the correct network byte ordering is observed)
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(PORT);       // port to listen on

    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket\n");
        exit(EXIT_FAILURE);
    }

    // listen for client connections (pending connections get put into a queue)
    if (listen(server_socket, NUM_CONNECTIONS) == -1) {
        perror("Error listening on socket\n");
        exit(EXIT_FAILURE);
    }

    // server loop
    while (true)
    {
        // accept connection to client
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1)
        {
            perror("Error accepting client\n");
        }else
        {
            // print out client being opened
            printf("Socket number of client opened: %d\n", client_socket);
            handle_client(&client_socket);
        }
    }
}


/************************************************************************
 * handle client
 ************************************************************************/

void * handle_client(void *arg)
{
    // initialize variables
    int client_socket = *( (int *) arg );

    int input;
    int keep_going = true;

    // read int from client
    read(client_socket, &input, sizeof(int));


    printf("Client connected.\n");

    // convert int from client
    input = htonl(input);

    // get number of steps
    int algorithmSteps = ThreeAPlusOne(input);

    // send result back to client
    write(client_socket, &algorithmSteps, sizeof(int));

    sleep(1);

    // print out info
    printf("number recieved from client:%d\n", input);
    printf("number of steps sent back: %d\n", algorithmSteps);


    //cleanup
    if (close(client_socket) == -1)
    {
        perror("Error closing socket\n");
        exit(EXIT_FAILURE);
    }
    else
    {
      // print out client being closed
      printf("Socket number of client closing: %d\n", client_socket);
    }

    return 0;
}

int ThreeAPlusOne(int input)
  {
    int counter = 0;
    int current = input;

    while (current != 1)
    {
        counter++;
        if (current % 2) {
            current = (current*3) + 1;
        }
        else {
            current >>= 1;
        }
    }
    return counter;
  }
