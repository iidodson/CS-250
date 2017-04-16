//  Indya Dodson
//  CS-250-Homework4.c
//  4/13/17

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 512

int main(void)
{
    struct sockaddr_in servaddr, cliaddr;
    int sock, read, i;
    socklen_t clilen = sizeof(cliaddr);
    char input[BUFFER_SIZE];
    char output[BUFFER_SIZE];
    char path[BUFFER_SIZE];
    FILE *fp;
    time_t rawtime;
    struct tm * timeinfo;
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("Socket error");
        exit(1);
    }
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1337);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //1. Bind to any IP Address on port 1337/udp
    
    if (bind(sock, (struct sockaddr* ) &servaddr, sizeof(servaddr)) < 0){
        perror("Bind error");
        exit(1);
    }

    
    while(1)
    {
        //rocess queries from the client (you are not required to validate commands) and execute in the operating system.
        read = recvfrom(sock, input, BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &clilen);
        if ( read < 0) {
            perror("Recv error");
            exit(-1);
        }
        
        output[read] = '\0';
        
        fp = popen(output, "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }
        
        
        //You'll likely need to parse the output so that you only return time to the client<br>
        while (fgets(path, sizeof(path)-1, fp) != NULL) {
            printf("%s\n", path);
            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            printf ( "Executed at: %s", asctime (timeinfo) );
            
        }
        
        //Return the results to the client
        if (sendto(sock, path, BUFFER_SIZE, 0, (struct sockaddr*) &cliaddr, clilen) < 0){
            perror("Sendto error");
            exit(1);
        }
        
    }

    pclose(fp);
    close(sock);
    return 0;
}
