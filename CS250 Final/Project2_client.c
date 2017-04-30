//  Indya Dodson
//  Project2_client.c
//  4/29/17

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define LEN 150

int main( int argc, char **argv ) {
    int sock, sent, read;
    struct sockaddr_in server;
    char input[BUFFER_SIZE];
    char output[BUFFER_SIZE];
    struct hostent *hp;
    struct tm lt;
    char stamp[LEN];
    
    //Accept hostname or IP address & port number as parameters
    if (argc < 3 || argc > 3) {
        perror("Enter hostname and port\n");
        exit(-1);
    }
    
    //Open a UDP socket (report errors).
    if ((sock = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
        perror("Socket error.");
        exit(-1);
    }
    
    //Convert the hostname into an IP addresxs. Report "hostname error" if unsuccessful.
    if ((hp = gethostbyname(argv[1])) == 0) {
        printf("Host error\n");
        exit(1);
    }
    
    memcpy( &server.sin_addr.s_addr, hp->h_addr, hp->h_length);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    bzero(&server.sin_zero, 0);
    
    //Continue operating until the user terminates the program.
    while (1){
        printf("Please enter the message you would like to send:\n");
        scanf("%c", input);
        
        time_t t = time(0);
        localtime_r(&t, &lt);
        strftime(stamp, sizeof(stamp), " %a %b %d &T %z %Y", &lt);
        
        strcat(input, stamp);
        memset(stamp, 0, sizeof stamp);

        //Send the message (report errors). a. To ensure that we can track messages, append a timestamp to the message before sending.
        sent = sendto(sock, input, strlen(input), 0, (struct sockaddr*) &server, sizeof(server));
        
        if (sent < 0) {
            perror("Sending error/n");
            exit(-1);
        }
        else{
            printf("Message sent\n");
        }
        
        //Receive messages from the server (report errors). a. Do not display messages that you sent.
        read = recvfrom(sock, input ,BUFFER_SIZE, 0, NULL, NULL);
        if ( read < 0) {
            perror("Recv error");
            exit(-1);
        }
        else {
            printf("Message recieved\n");
        }
    
    }
    
    return(0);
}



