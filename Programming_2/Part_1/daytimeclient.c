#include "daytimeclient.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main() {
    char input[100];              // buffer for user input
    int client_socket;            // client side socket
    char* time_server_ip;
    struct sockaddr_in server_address;  // client socket naming struct
    char c;

    printf("Daytime client\n");

    // create an unnamed socket, and then name it
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // create addr struct
    server_address.sin_family = AF_INET;
    // getting the string of the ip of server, given SERVER_ADDR
    time_server_ip = inet_ntoa((struct in_addr)(gethostbyname(SERVER_ADDR)->h_addr_list[0]));
    server_address.sin_addr.s_addr = inet_addr(time_server_ip);
    server_address.sin_port = htons(PORT);

    // connect to server socket
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server!\n");
        exit(EXIT_FAILURE);
    }

    while (true)
      {

            // make the request to the server
            write(client_socket, input, sizeof(char));
            // get the result
            read(client_socket, &c, sizeof(char));
            if (c == '*') {
                printf("\n\nTime printed!\n");
                close(client_socket);
                exit(EXIT_SUCCESS);
            }
            printf("%c", c);

      }

    return EXIT_SUCCESS;
}
