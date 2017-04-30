//  Indya Dodson
//  Project2_server.c
//  4/29/17

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>

#define BUFFER_SIZE 512

//Add the necessary logic so that the server can be added as a daemon/service in the operating system.
static void mydaemon(){
    pid_t pid;
    pid = fork();
    
    if (pid < 0)
        exit(1);
    
    if (pid > 0)
        exit(1);
    
    if (setsid() < 0)
        exit(1);
    
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    
    pid = fork();
    
    if (pid < 0)
        exit(1);
    
    if (pid > 0)
        exit(1);
    
    umask(002);
    
    chdir("/");
    
    int i;
    for (i = sysconf(_SC_OPEN_MAX); i>=0; i--)
    {
        close (i);
    }
    
    openlog("mydaemon",LOG_PID,LOG_DAEMON);

}

int main(int argc, char* argv[]){
    
    mydaemon();
    
    struct sockaddr_in servaddr, cliaddr;
    int broadcast;
    int sock, read, pid;
    socklen_t clilen = sizeof(cliaddr);
    char input[BUFFER_SIZE];
    
    //Accept hostname or IP address & port number as parameters
    if (argc < 3 || argc > 3) {
        perror("Enter hostname and port\n");
        exit(-1);
    }

   // Open a UDP Socket (report errors).
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        perror("Socket error");
        exit(-1);
    }
    else{
        syslog(LOG_INFO, "Socket created from host %s", argv[2]);
    }
    
    //Send the received message to all clients using a broadcast. a. There are two valid ways to send using a broadcast address. Either is fine for this project. b. Ensure that you set the correct socket structure options so that you have permissions to use a broadcast.
    broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(-1);
    }
   
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    while(1){
        if (bind(sock, (struct sockaddr* ) &servaddr, sizeof(servaddr)) < 0){
            perror("Bind error");
            exit(1);
        }
        pid=fork();
        if (pid == 0) {
            while (1) {
                //Receive messages from clients (report errors).
                if ((recvfrom(sock, input, BUFFER_SIZE, 0, (struct sockaddr*)&cliaddr, &clilen) < 0)) {
                    perror("Recv error");
                    exit(-1);
                }
                else{
                    printf("Message recieved.");
                }
                
                if (sendto(sock, input, BUFFER_SIZE, 0, (struct sockaddr*) &cliaddr, clilen) < 0){
                    perror("Sendto error");
                    exit(1);
                }
                else{
                    printf("Message sent.");
                }
            }
            exit(0);
        }
        else{
            close(sock);
            syslog(LOG_INFO, "Connection terminated");
            closelog();
        }
    }
    return 0;
}
