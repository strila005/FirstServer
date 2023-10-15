#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "header_client.h"

#define PORT 8080
#define INET_ADDR "127.0.0.1"
#define ID 123123

int main() {
    int sockfd, size=1024, i=0;
    struct sockaddr_in server_addr;
    char sendbuf[1024], buffer[1024], message[]="Wrong ID. Try again."; 
    FILE *file;

    //Create a socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //Set the server address and port
    memset(&server_addr, '0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(INET_ADDR);

    // Connecting to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to the server");
        exit(EXIT_FAILURE);
    }
    else printf("\033[1m\nConnected to the server! \n\n\033[0m"); //bold text

    //Open file
    if ((file = fopen("SecretChat.txt", "w")) == NULL) {
        printf("File didn`t open.\n");
        return 1;
    }
    else printf("File was opened. All messages will be recorded\n");

    //Find out the center of terminal window
    int rows, columns;
    getTerminalSize(&rows, &columns);

    //Send message
    sendMessage(sockfd, file, sendbuf, buffer, size, columns, 1); 
    //Receive a response from the server
    receiveMessage(sockfd, file, buffer, size, columns, 1);
    sendMessage(sockfd, file, sendbuf, buffer, size, columns, 1); //123123
    while (i<3){
        receiveMessage(sockfd, file, buffer, size, columns, 2);
        if (strcmp(message, buffer) == 0) {
            sendMessage(sockfd, file, sendbuf, buffer, size, columns, 1);  
        }
        else{
            break;
        }
        i++;
    }
    sendMessage(sockfd, file, sendbuf, buffer, size, columns, 1); // anything
    receiveMessage(sockfd, file, buffer, size, columns, 1);
    sendMessage(sockfd, file, sendbuf, buffer, size, columns, 1); // 8484848
    receiveMessage(sockfd, file, buffer, size, columns, 1);
    sendMessage(sockfd, file, sendbuf, buffer, size, columns, 1); // 753422
    receiveMessage(sockfd, file, buffer, size, columns, 1);
    division(ID, buffer, size);                            //division
    sendMessage(sockfd, file, sendbuf, buffer, size, columns, 2);

    //Close file
    if (fclose(file)==EOF) printf("Error!");
    else printf("\x1b[0m\033[1mFile was closed. All messages were recorded.");

    //Close socket
    close(sockfd);

    return 0;
}
