#include "client.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main() {
    char input[100];
    char operator[100];
    char negChar;
    double result;
    int client_socket;                 // client side socket
    struct sockaddr_in client_address;  // client socket naming struct
    int infoToPass[3]; // holds 1-2 numbers and a flag for operator
    int firstNumber, secondNumber, operatorFlag;
    bool firstNumberNeg = false;


    printf("Compute client\n");

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
        // check user doesn't want to quit
        printf("Input q if finished, press ENTER if not: ");
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "q") == 0) {
            close(client_socket);
            printf("\nClosing session of client\n");
            exit(EXIT_SUCCESS);
         }

        // get first number
        printf( "Input first number: ");
        // read string
        scanf("%d", &firstNumber);

        //get negative flag
        printf("Would you like the number negative (Y/N)? ");
        scanf(" %c", &negChar);

        //convert to lowercase for ease of use
        negChar = tolower(negChar);

        // assign negative flag
        if (negChar == 'y')
        {
          firstNumberNeg = true;
          firstNumber *= -1;
        }
        else if (negChar == 'n')
        {
          firstNumberNeg = false;
        }

        // get operator
        printf("Input operator (+, -, *, /, ^, sqrt): ");
        // read string
        scanf("%s", operator);

        //convert operator to correct flag
        if (strcmp(operator, "+") == 0)
        {
          operatorFlag = ADD;
        }
        else if (strcmp(operator, "-") == 0)
        {
          operatorFlag = SUBTRACT;
        }
        else if (strcmp(operator, "*") == 0)
        {
          operatorFlag = MULTIPLY;
        }
        else if (strcmp(operator, "/") == 0)
        {
          operatorFlag = DIVIDE;
        }
        else if (strcmp(operator, "^") == 0)
        {
          operatorFlag = RAISE;
        }
        else if (strcmp(operator, "sqrt") == 0)
        {
          operatorFlag = SQRT;
        }
        else
        {
          operatorFlag = INCORRECT_OP;
          printf("Incorrect operator, starting over\n");
          close(client_socket);
          printf("\nClosing session of client\n");
          exit(EXIT_SUCCESS);
        }

        // get second
        if ( operatorFlag != SQRT)
        {
          printf("Input second number: ");
          // read string
          scanf("%d", &secondNumber);

          //get negative flag
          printf("Would you like the number negative (Y/N)? ");
          scanf(" %c", &negChar);

          //convert to lowercase for ease of use
          negChar = tolower(negChar);

          // assign negative flag
          if (negChar == 'y')
          {
            secondNumber *= -1;
          }
        }

        // real numbers will always occur with forcing input for the numbers to be ints

        // check no negative number for sqrt
        if (firstNumberNeg && operatorFlag == SQRT)
        {
          printf("Trying to do a square root on a negative number, invalid\n");
          close(client_socket);
          printf("\nClosing session of client\n");
          exit(EXIT_SUCCESS);
        }

        // check that the second number is not 0 if dividing
        if ( operatorFlag==DIVIDE && secondNumber == 0)
        {
          printf("Division by zero, invalid\n");
          close(client_socket);
          printf("\nClosing session of client\n");
          exit(EXIT_SUCCESS);
        }

        // put in info and convert
        infoToPass[0] = htonl(firstNumber);
        infoToPass[1] = htonl(operatorFlag);
        infoToPass[2] = htonl(secondNumber);


        // make the request to the server
        write(client_socket, &infoToPass, sizeof(infoToPass));

        // get the result
        recv(client_socket, &result, sizeof(result), 0 );

        printf("Result of math: %2.3f\n", result);

        close(client_socket);
        printf("\nClosing session of client\n");
        exit(EXIT_SUCCESS);

    }
    close(client_socket);

    return EXIT_SUCCESS;
}
