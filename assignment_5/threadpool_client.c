#include "threadpool_client.h"
#include "threadpool.h"

/* ******************************************************* */
/* main()                                                  */
/* ******************************************************* */
int main() {
    char input[100];                    // buffer for user input
    int client_socket;                  // client side socket
    struct sockaddr_in client_address;  // client socket naming struct
    int taskAmount;
    int numberToPass;
    int recievedNum;

    // create threadpool with THREADS_IN_POOL threads
    threadpool pool = threadpool_create();

    printf("stress client\n");

        for (taskAmount=1; taskAmount<=NUMBER_TASKS; taskAmount++)
        {
            // in each loop, execute three_a_plus_one_wrapper in a thread from the pool
            threadpool_add_task(pool, task_copy_arguments, three_a_plus_one_wrapper, (void*)&taskAmount);
        }
        sleep(500);

    exit(EXIT_SUCCESS);
}


/* ******************************************************* */
/* three_a_plus_one_wrapper()                              */
/* ******************************************************* */
void three_a_plus_one_wrapper(void *number_ptr)
{
    // original
    int number = *((int*)number_ptr);
    int recievedNum, numberToPass;
    //int taskAmount;

    int client_socket;                  // client side socket
    struct sockaddr_in client_address;  // client socket naming struct

    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    client_address.sin_port = htons(PORT);

    // connect to server socket
    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1) {
        perror("Error connecting to server!\n");
        exit(EXIT_FAILURE);
    }

    //taskAmount = atoi(number);
    numberToPass = htonl(number);

    // make the request to the server
    write(client_socket, &numberToPass, sizeof(int));

    // get the result
    read(client_socket, &recievedNum, sizeof(int));

    // original
    printf("number:%d ----------> steps:%d\n", number, recievedNum);

    //sleep(500);
    close(client_socket);

}


/* ******************************************************* */
/* three_a_plus_one() - nonrecursive                       */
/* ******************************************************* */
int three_a_plus_one(int input)
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
/* three_a_plus_one_rec() - recursive                          */
/* ******************************************************* */
int three_a_plus_one_rec(int number) {
    int new_number;

    if (number == 1) {
        return 0;
    }

    if (number % 2) {
        new_number = 3 * number + 1;
    } else {
        new_number = number / 2;
    }

    return 1 + three_a_plus_one(new_number);
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
