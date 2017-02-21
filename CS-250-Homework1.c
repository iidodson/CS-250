//  Indya Dodson
//  CS-250-Homework1.c
//  2/20/17

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main (int argc, char *argv[]) {
    struct sockaddr_in server;
    int sock_echo;
    char input[BUFFER_SIZE];
    char output [BUFFER_SIZE];
    int length;
    
    if((sock_echo = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        err_sys("Socket error.");
        exit(-1);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(13);
    server.sin_addr.s_addr = inet_addr(argv[1]);
    bzero(&server.sin_zero, 0);
    
    //1. Handle whether the connection is made or not
    if((connect(sock_echo, (struct sockaddr *)&server, sizeof(server))) < 0){
        err_sys("Connection error.");
        exit(-1);
    }
    else{
        //2. Output status to the user (e.g. connected)
        printf("Connected!");
    }
    
    while (1) {
        fgets(input, BUFFER_SIZE,stdin);
        send(sock_echo, input, strlen(input), 0);
        
        //3. Handle replies from the server
        length = recv(sock_echo, output, BUFFER_SIZE, 0);
        //taken from https://www.youtube.com/watch?v=FRm9nk9ooC8
        output[length] = '\0';
        printf("%s\n", output);
        
    }
    
    //4. Teardown the connection properly when existing the client.
    close(sock_echo);
    
}
