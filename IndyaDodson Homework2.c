//  Indya Dodson
//  CS-250-Homework1.c
//  3/20/17

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 128

int main (int argc, char *argv[]) {
    
    struct sockaddr_in server, client;
    int server_fd, client_fd, read;
    int sockaddr_len = sizeof(struct sockaddr_in);
    char data[BUFFER_SIZE];
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket error.");
        exit(-1);
    }
    
    //Bind to any IP Address on port 23/tcp
    server.sin_family = AF_INET;
    server.sin_port = htons(23);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&server.sin_zero, 0);
    
    //Handle whether the connection is made or not
    if((bind(server_fd, (struct sockaddr *)&server, sizeof(server))) < 0){
        perror("Bind error\n");
        exit(-1);
    }
    
    if((listen(server_fd, MAX_CLIENTS)) < 0){
        perror("Listen error\n");
        exit(-1);
    }
    
    while (1) {
        socklen_t client_size = sizeof(client);

        if((client_fd = accept(server_fd, (struct sockaddr *)&client, &client_size)) < 0){
            perror("Connection error\n");
            exit(-1);
        }
        
        read = 1;
        
         while (read) {
             
             //Process input from the client (you are not required to validate commands) and execute in the operating system
             read = recv(client_fd, data, BUFFER_SIZE, 0);
             
             if(read){
                 //Return the results to the client
                 send(client_fd, data, read, 0);
                 data[read] = '\0';
                 printf("%s was sent to the client", data);
             }
             
         }
        
        close(client_fd);
    }
    //Teardown the connection properly when the client disconnects
    close(server_fd);

}
