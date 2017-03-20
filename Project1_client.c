//  Indya Dodson
//  CS-250-Project2.c
//  3/21/17

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
    int sock;
    char input[BUFFER_SIZE];
    char output [BUFFER_SIZE];
    int length;
    
    
    char *hostname = argv[1];

    int port = atoi(argv[2]);
    char ip[100];
    char filename[100];
    
    //Accept a hostname and port as command arguements (i.e., client hostname 80).
    /* check command line arguments */
    if (argc < 3 || argc > 3) {
        err_sys("Enter hostname and port as command arguements.");
        exit(-1);
    }
    
    //Convert the hostname into an IP address. Report "hostname error" if unsuccessful.
    get_ip(hostname , ip);

    //Open a socket connection with the server. Report "unable to connect" on failure.
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        err_sys("Unable to connect");
        exit(-1);
    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(13);
    server.sin_addr.s_addr = inet_addr(argv[1]);
    bzero(&server.sin_zero, 0);
    
    if((connect(sock, (struct sockaddr *)&server, sizeof(server))) < 0){
        err_sys("Connection error.");
        exit(-1);
    }
    else{
        printf("Connected!");
    }
    
    //Read a command from the user. You can assume the command is the file name that the user wants to retrieve (e.g. index.html).
    
    printf( "Enter the filename you want to retrieve:");
    scanf("%c", input);

    //Send data to the server (using your protocol). Report "server not responding" if the connection appears to be dead.
    while (1) {
        fgets(input, BUFFER_SIZE,stdin);
        send(sock, input, strlen(input), 0);
        
        //Wait for the server's response. Be sure to implement behavior that avoids blocking conditions!
        length = recv(sock, output, BUFFER_SIZE, 0);
        
        //Handle data sent by the server. The contents of the HTTP resource should be printed to stdout correctly. For example, if the target resource consists of two lines separated by a newline then the client must render correctly [Be sure to test at least a single line and a double line!].
        //taken from https://www.youtube.com/watch?v=FRm9nk9ooC8
        output[length] = '\0';
        printf("%s\n", output);
        
    }
    
    //Close the socket connection and exit the program.
    close(sock);
    
}

//######################################
//taken from: https://srishcomp.wordpress.com/2013/01/15/a-c-program-to-get-ip-address-from-the-hostname/
int get_ip(char * hostname , char* ip){
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ( (he = gethostbyname( hostname ) ) == NULL){
        err_sys("gethostbyname");
        return 1;
    }
    
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++){
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
    return 1;
}
//######################################

