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
    int server_fd;
    //int socket(int domain, int type, int protocol)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

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
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //3.Bind
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof server_addr);
    //int bind(int sockfd, struct sockaddr *my_addr, int addrlen);

    //4.Listen
    int backlog = 3;
    listen(server_fd, backlog);
    //int listen(int sockfd, int backlog); 
    printf("Server is listening on port %d, waiting Client to connect...\n", PORT);

    //5.Accept
    int new_socket = accept(server_fd, NULL, NULL);
    // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

    //6.Receive
    char buffer[1024] = {0}; 
    int valread = recv(new_socket, buffer, sizeof buffer, 0);
    //int recv(int sockfd, void *buf, int len, int flags);
    if(valread > 0){
        printf("Got message from Client : %s\n", buffer);
        if (strcmp(buffer, "ping") == 0){
            printf("Server: Got ping! Now preparing to reply pong...\n");
            
            const char *msg = "pong";
            send(new_socket, msg, strlen(msg), 0);
            // int send(int sockfd, const void *msg, int len, int flags);
    }
    else{
        printf("Server: Not received ping , but : %s\n", buffer);
    }
}
    else{
            printf("Client dissconnected or error. \n");
    }
    //7.Cleanup
    close(new_socket);
    close(server_fd);

    return 0;
}