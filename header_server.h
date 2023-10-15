#ifndef HEADER_CLIENT_H_
#define HEADER_CLIENT_H_

void cleanBuffer(){
    while((getchar())!='\n');
}

char sendMessage(int sockfd, char *sendbuf, int size, int condition){
    memset(sendbuf, 0, size);
    if (condition == 1){
        strcpy(sendbuf, "Wake up Neo...  The Matrix has you...  (You're connected to Morpheus' server. Send me your ID to proceed.)");
    }
    else if (condition == 2){
        strcpy(sendbuf, "Hello Vikhot Yulianna I need you to do something for me...");
    }
    else if (condition == 3){
        strcpy(sendbuf, "Wrong ID. Try again.");
    }
    else if (condition == 4){
        strcpy(sendbuf, "Please disconnect, then reprogram your software to display your name including diacritics using the function SetConsoleOutputCP(CP_UTF8). Display messages from me one letter at a time (wait a few milliseconds after each letter). Then reconnect and after reading this message send me the code 8484848.");
    }
    else if (condition == 5){
        strcpy(sendbuf, "You sent me the correct code. Now disconnect and reprogram your software to show all my messages in green color and all your messages in blue color. If you don't know how, check the documentation for excercise 2.5.1. Then reconnect and send me the code 753422.");
    }
    else if (condition == 6){
        strcpy(sendbuf, "You sent me the correct code. Now send me the result of the following. Compute the sum of first 5 digits of your student ID, then divide it by the fifth digit and send me the remainder of the division. The operation muse be programed using a function using the ID as an argument. It can't be hard-coded. WARNING - If the fifth digit is zero, use 9 instead. The result must be computed by your software!");
    }
    if (send(sockfd, sendbuf, strlen(sendbuf), 0) < 0) {
        perror("send() error");
        exit(EXIT_FAILURE);
    }
    return *sendbuf;
}

char receiveMessage(int sockfd, char *buffer, int size, int condition){
    memset(buffer, 0, size);
    if (recv(sockfd, buffer, size, 0) < 0) {
        perror("recv() error");
        exit(EXIT_FAILURE);
    }
    buffer[strlen(buffer)] = '\0';
    printf("Received message: %s\n", buffer);
    return *buffer;
}

#endif