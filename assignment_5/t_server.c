#include "t_server.h"
#include "threadpool.h"
#include <pthread.h>

// create global mutex lock
pthread_mutex_t lock;

// create global max num of threads
int MAX_THREADS = 30;

// create global num of threads var
int numThreads = 0;
/************************************************************************
 * MAIN
 ************************************************************************/

int main(int argc, char *argv[]) {
    int server_socket;                 // descriptor of server socket
    int client_socket;


    struct sockaddr_in server_address; // for naming the server's listening socket
    pthread_t thread;                  // create thread id

    threadpool tpool = threadpool_create();

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

    //init mutex
    pthread_mutex_init(&lock, NULL);

    // server loop
    while (true)
    {
        // accept connection to client
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1)
        {
            perror("Error accepting client\n");
        }else
        {
            if( numThreads < MAX_THREADS )
            {
               // lock mutex
               pthread_mutex_lock(&lock);

               // increase thread number
               numThreads ++;

               // print out client being opened
               printf("Socket number of client opened: %d\n", client_socket);

               printf("Num of threads before add task: %d\n", numThreads);

               //create new threadpool for every incoming task
               threadpool_add_task(tpool, task_copy_arguments, handle_client, (void *)&numThreads);

            }else{numThreads --;}
        }
        sleep(500);
    }
}


/************************************************************************
 * handle client
 ************************************************************************/

void handle_client(void *arg)
{
    int input;
    int keep_going = true;

    // initialize variables
    int client_socket = *( (int *) arg );
    printf("Num of Threads: %d\n", numThreads );
    // decrease number of threads
    numThreads --;

    // unlock mutex
    pthread_mutex_unlock(&lock);

    // read int from client
    read(client_socket, &input, sizeof(int));

    // convert int from client
    input = htonl(input);

    // get number of steps
    int algorithmSteps = ThreeAPlusOne(input);

    // send result back to client
    write(client_socket, &algorithmSteps, sizeof(int));

    // print out info
    printf("number recieved from client:%d\n", input);
    printf("number of steps sent back: %d\n", algorithmSteps);

    // cleanup
    if (close(client_socket) == -1)
    {
        perror("Error closing socket\n");
        pthread_exit(NULL);
    }
    else
    {
      // print out client being closed
      printf("Socket number of client closing: %d\n", client_socket);
    }

    //return 0;
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

/* ******************************************************* */
/* prepare arguments for thread function                   */
/* ******************************************************* */
void *task_copy_arguments(void *args_in)
{
    void *args_out;

    args_out = malloc(sizeof(int));
    *((int*)args_out) = *((int*)args_in);

    return args_out;
}
