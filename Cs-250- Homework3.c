//  Indya Dodson
//  CS-250-Homework3.c
//  4/1/17

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main( int argc, char **argv ) {
    int sock, sent, read;
    struct sockaddr_in server;
    struct hostent *hp;
    char input[BUFFER_SIZE];
    char output[BUFFER_SIZE];
    
    if (argc < 3 || argc > 3) {
        perror("Enter hostname and port\n");
        exit(-1);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    
    if ((sock = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
        perror("Socket error.");
        exit(-1);
    }
    
    if ((hp = gethostbyname(argv[1])) == 0) {
        printf("Host error\n");
        exit(1);
    }
    
    memcpy( &server.sin_addr.s_addr, hp->h_addr, hp->h_length);
    
    printf("Please enter the message you would like to send:\n");
    scanf("%c", input);
    
    bzero(&server.sin_zero, 0);
    sent = sendto(sock, input, strlen(input), 0, (struct sockaddr*) &server, sizeof(server));
    
    // 1. Output status to the user (e.g. message sent)
    if (sent < 0) {
        perror("Sending error/n");
        exit(-1);
    }
    else{
        printf("Message sent\n");
    }
    
    // 2. Handle replies from the server
    read = recvfrom(sock, input ,BUFFER_SIZE, 0, NULL, NULL);
    if ( read < 0) {
        perror("Recv error");
        exit(-1);
    }
    
    // 3. Print the reply to screen
    output[read] = '\0';
    printf("%s\n", output);
    
    return(0);
}
