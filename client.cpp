#include <string.h>
#include <stdio.h>
#include <unistd.h>     // for close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // IPv4 specific
#include <arpa/inet.h>

#define PORT 3490  

int main() {
    //1. Create socket
    int sock;
    //int socket(int domain, int type, int protocol)
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //2.Prepare address
    struct sockaddr_in server_addr;
    // struct sockaddr_in{
    //     short sin_family;
    //     unsigned short sin_port;
    //     struct in_addr sin_addr;
    //     char sin_zero[8];
    // }
    // struct in_addr{
    //     unsigned long s_addr;
    // }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    //3.connect
    connect(sock, (struct sockaddr *)&server_addr, sizeof server_addr);


    //4.Send
    const char *msg = "ping";
    send(sock, msg, strlen(msg), 0);


    //5.Receive
    char buffer[1024] = {0}; 
    int valread = recv(sock, buffer, sizeof buffer, 0);
    //int recv(int sockfd, void *buf, int len, int flags);
    if(valread > 0){
        printf("Got message from Server : %s\n", buffer);
        if (strcmp(buffer, "pong") == 0){
            printf("Client: Got pong!");
}
    else{
            printf("Server dissconnected or error. \n");
    }
}
    //6.Cleanup
    close(sock);

    return 0;
}