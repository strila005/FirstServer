#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "header_server.h"

#define PORT 8080

// A function to process the client
void *handle_client(void *client_socket) {
    int client_fd = *((int *)client_socket);
    char buffer[1024], sendbuf[1024], ID[] = "123123"; 
    int size = 1024, condition, i=0; 

    receiveMessage(client_fd, buffer, size, condition);
    sendMessage(client_fd, sendbuf, strlen(sendbuf), 1);
    while (i<3) {
        receiveMessage(client_fd, buffer, size, condition);
        if (strcmp(ID, buffer) == 0) {
            sendMessage(client_fd, sendbuf, strlen(sendbuf), 2); 
            break;
            }
        else {
            sendMessage(client_fd, sendbuf, strlen(sendbuf), 3);  //wrong ID
        }
        i++;
    }
    receiveMessage(client_fd, buffer, size, condition); 
    sendMessage(client_fd, sendbuf, strlen(sendbuf), 4);
    receiveMessage(client_fd, buffer, size, condition); // 8484848
    if (strcmp("8484848", buffer) == 0) {
            sendMessage(client_fd, sendbuf, strlen(sendbuf), 5);
    } else{
        close(client_fd);
        printf("Client's socket was closed\n\n");
        free(client_socket);
        return NULL;
        }
    receiveMessage(client_fd, buffer, size, condition); // 753422
    if (strcmp("753422", buffer) == 0) {
            sendMessage(client_fd, sendbuf, strlen(sendbuf), 6);
    }   else{
        close(client_fd);
        printf("Client's socket was closed\n\n");
        free(client_socket);
        return NULL;
        }
    receiveMessage(client_fd, buffer, size, condition);
    
    // Close socket
    close(client_fd);
    printf("Client's socket was closed\n\n");
    free(client_socket);

    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024], sendbuf[1024]; int size = 1024;

    // Creating a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Setting up the server settings
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Socket binding to IP address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }
    printf("\nThe server is ready to accept connections...\n\n");

    while (1) {
        // Accepting connection
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("Accepting failed");
            exit(EXIT_FAILURE);
        }
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }
        printf("Client connected %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a thread to process the client
        pthread_t client_thread;
        int *client_fd = (int *)malloc(sizeof(int));
        *client_fd = client_socket;

        if (pthread_create(&client_thread, NULL, handle_client, (void *)client_fd) != 0) {
            perror("Error creating thread");
            exit(1);
        }
    }
    // Closing the server socket
    close(server_socket);

    return 0;
}
