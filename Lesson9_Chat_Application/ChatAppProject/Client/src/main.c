#include "../inc/main.h"

void initialize_client()
{
    // Client initialization code goes here
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    if(connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to chat server. Type messages and press Enter to send.\n");
}

void *receive_messages(void *arg)
{
    int fd = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int nbytes;

    while((nbytes = recv(fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[nbytes] = '\0';
        printf("%s", buffer);
    }

    return NULL;
}

int main()
{
    initialize_client();

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, &sock_fd);

    while(fgets(message, BUFFER_SIZE, stdin)) {
        send(sock_fd, message, strlen(message), 0);
    }

    close(sock_fd);
    return 0;
}