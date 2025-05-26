#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SOCKET_PATH "/tmp/unix_dgram_socket"
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    char *message = "Hello from client";
    char buffer[BUFFER_SIZE];
    struct sockaddr_un servaddr;

    // Create socket
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
        printf("Socket creation failed\n");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sun_family = AF_UNIX;
    strncpy(servaddr.sun_path, SOCKET_PATH, sizeof(servaddr.sun_path) - 1);

    // Send message to server
    sendto(sockfd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Message sent\n");

    // Receive response from server
    socklen_t len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    close(sockfd);
    return 0;
}
