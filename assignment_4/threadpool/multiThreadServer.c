#include "multiThreadServer.h"

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
            // lock mutex
            pthread_mutex_lock(&lock);
            printf("Accepted client\n");
            //create new thread for every incoming client
            if(pthread_create( &thread, NULL, handle_client, &client_socket ) == -1)
            {
              perror("Error creating thread\n");
            }

            // detach the thread
            pthread_detach(thread);


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
    // unlock mutex
    pthread_mutex_unlock(&lock);
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

    // cleanup
    if (close(client_socket) == -1)
    {
        perror("Error closing socket\n");
        pthread_exit(NULL);
    }
    else
    {
        printf("\nClosed socket to client, exit\n");
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
