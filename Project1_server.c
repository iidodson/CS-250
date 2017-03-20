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
    
    if (argc < 2) {
        perror("Enter port.");
        exit(-1);
    }
    
    //Create a listening socket based on the command arguments (e.g. server 80)
    int unsigned port = atoi(argv[1]);
    struct sockaddr_in server, client;
    int server_fd, client_fd, read;
    int sockaddr_len = sizeof(struct sockaddr_in);
    char* data[BUFFER_SIZE];
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket error.");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&server.sin_zero, 0);

    //Accept a connection from a client.
    if((bind(server_fd, (struct sockaddr *)&server, sizeof(server))) < 0){
        perror("Bind error\n");
        exit(-1);
    }

    if((listen(server_fd, MAX_CLIENTS)) < 0){
        perror("Listen error\n");
        exit(-1);
    }
    
    printf("Server is listening on %d\n", port);
    
    while (1) {
        socklen_t client_size = sizeof(client);
  
        if((client_fd = accept(server_fd, (struct sockaddr *)&client, &client_size)) < 0){
            perror("Connection error\n");
            exit(-1);
        }
        
        read = 1;

        while (read) {
            if(read){
                
                //Read a HTTP request from a connected client. Because we are building a very simple HTTP server, just handle the incoming file name. You can assume that the target file (e.g. index.html) is in the same directory as the server.
                FILE *file1 = fopen(data, "a");
                if(file1 == NULL)
                    printf("File error\n");
                else
                {
                    bzero(data, BUFFER_SIZE);
                    int file_block = 0;
                    while((file_block = recv(client_fd, data, BUFFER_SIZE, 0)) > 0){
                        int write = fwrite(data, sizeof(char), file_block, file1);
                        if(write < file_block)
                        {
                            perror("Write error\n");
                        }
                        bzero(data, BUFFER_SIZE);
                        if (file_block == 0 || file_block != BUFFER_SIZE)
                        {
                            break;
                        }
                        
                    }
                   
                    if(send(client_fd, data, file_block, 0) < 0)
                    {
                        perror("Error sending file\n");
                        exit(1);
                    }
                    else{
                        data[read] = '\0';
                        printf("%s was sent to the client", data);

                    }
                }

            }
            
        }
    }
    
    return 0;
}
