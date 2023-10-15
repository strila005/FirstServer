#ifndef HEADER_SERVER_H_
#define HEADER_SERVER_H_

void cleanBuffer(){
    while((getchar())!='\n');
}

void getTerminalSize(int *rows, int *columns) {
    struct winsize size;
    ioctl(0, TIOCGWINSZ, &size); // terminal size
    *rows = size.ws_row;
    *columns = size.ws_col;
}

char sendMessage(int sockfd, FILE *file, char *sendbuf, char *buffer, int size, int x, int condition){
    int i=0, a=0; char c;
    if (condition!=1) strcpy(sendbuf, buffer);
    else {
        printf("\x1b[34m");   // blue text color
        while ((c = getchar()) != '\n' && i < size - 1) {
            sendbuf[i++] = c;
        }
        sendbuf[i] = '\0';
    } 
    if (send(sockfd, sendbuf, strlen(sendbuf), 0) < 0) {
        perror("send() error");
        exit(EXIT_FAILURE);
    }
    if (condition!=3) {
        fprintf(file, "Yulianna's message: "); 
        for (i=0; sendbuf[i]!='\0'; i++){
            if (!(i%75)) fputc('\n', file);
            c=sendbuf[i];
            fputc(c, file);
        }
        fputc('\n', file);
        printf("\x1b[0mMessage was sent\n");  // standart text color
    }  
    else if (condition==3){
        printf("\n\033[%dCDecoded chifer was sent\n", x/2); 
        fprintf(file, "Decoded chifer was sent\n");}
    return *sendbuf;
}

char receiveMessage(int sockfd, FILE *file, char *buffer, int size, int x, int condition){
    int i; char c;
    memset(buffer, 0, size);
    if (recv(sockfd, buffer, size, 0) < 0) {
        perror("recv() error");
        exit(EXIT_FAILURE);
    }                                              
    fprintf(file, "\nServer: "); 
    for (i=0; buffer[i]!='\0'; i++){
        if (!(i%75) && buffer[i]) {
            fputc('\n', file);}
        c=buffer[i];                    //writing to the file
        fputc(c, file);
    }
    fprintf(file, "\n\n");
    printf("\033[%dC\x1b[0mServer message: \x1b[32m", x/2);
    for (int i = 0; buffer[i] != '\0'; i++) { 
        if (!(i%(x/2))) printf("\n\033[%dC", x/2);
        printf("%c", buffer[i]);  
        fflush(stdout); 
        usleep(100000);       
    }
    printf("\n");
    return *buffer;
}

char division(int ID, char *buffer, int size) {
    int i, sum = 0;
    memset(buffer, 0, size);
    char strX[7];
    sprintf(strX, "%d", ID);
    int divisor = strX[4] - '0'; // Convert character to integer
    if (divisor==0) divisor=9;
    for (i = 0; i < 5; i++) {
        sum += strX[i] - '0'; // Convert character to integer
    }
    printf("\x1b[34mMessage: %d\n", sum % divisor);
    sprintf(buffer, "%d", sum % divisor);
    return *buffer;
}

#endif