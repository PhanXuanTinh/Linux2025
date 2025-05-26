#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_stream_socket"
#define BUFFER_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_un serv_addr;
    char *message = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path) - 1);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    // Send message to server
    send(sock, message, strlen(message), 0);
    printf("Message sent\n");

    // Receive response from server
    read(sock, buffer, BUFFER_SIZE);
    printf("Received: %s\n", buffer);

    close(sock);
    return 0;
}
