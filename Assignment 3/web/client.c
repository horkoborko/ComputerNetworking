#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main() {
    char input[100];                    // buffer for user input
    int client_socket;                  // client side socket
    struct sockaddr_in client_address;  // client socket naming struct
    int number;
    int numberToPass;
    int recievedNum;

    printf("Echo client\n");

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

    while (TRUE) {
        printf("Input: ");
        // read string
        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "q") == 0) {
            close(client_socket);
            printf("\nDone!\n");
            exit(EXIT_SUCCESS);
         }

        number = atoi(input);
        numberToPass = htonl(number);

        // make the request to the server
        write(client_socket, &numberToPass, sizeof(int));
        // check for invalid input
        if (number < 1)
          {
            close(client_socket);
            printf("\nInvalid number\n");
            exit(EXIT_FAILURE);
          }
        // get the result
        read(client_socket, &recievedNum, sizeof(int));

        printf("Steps taken: %d\n", recievedNum);

        close(client_socket);
        exit(EXIT_SUCCESS);

    }

    return EXIT_SUCCESS;
}
