#include "echoserver.h"

/************************************************************************
 * MAIN
 ************************************************************************/

int main(int argc, char *argv[]) {
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int client_socket;
    pthread_t thread;                  // create thread id

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
    char input;
    int keep_going = true;

    while (keep_going)
    {
        // read char from client
        switch (read(client_socket, &input, sizeof(char)))
        {
            case 0:
                keep_going = false;
                printf("\nEnd of stream, returning ...\n");
                break;
            case -1:
                perror("Error reading from network!\n");
                keep_going = false;
                break;
        }

        printf("%c", input);

        // check if we terminate
        if (input == 'q')
        {
            keep_going = FALSE;
        }

        // send result back to client
        write(client_socket, &input, sizeof(char));

    }

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