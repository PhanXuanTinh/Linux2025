#include "../inc/main.h"

void broadcast(char *message, int sender_fd) {
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients[i] != 0 && clients[i] != sender_fd) {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg)
{
    int client_fd = *(int *)arg;
    free(arg); // Free the allocated memory for the client socket
    char buffer[BUFFER_SIZE];
    int nbytes;

    while ((nbytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[nbytes] = '\0';
        printf("Client %d: %s\n", client_fd, buffer);
        broadcast(buffer, client_fd);
    }

   // Remove client
   pthread_mutex_lock(&clients_mutex);
   for(int i = 0; i < MAX_CLIENTS; i++) {
       if(clients[i] == client_fd) {
           clients[i] = 0;
           break;
       }
   }
   pthread_mutex_unlock(&clients_mutex);

   close(client_fd);
   pthread_exit(NULL);
}

void initialize_server()
{
    // Server initialization code goes here 
    addr_len = sizeof(server_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
}

void server_running()
{
    while (1)
    {
        new_sock = accept(server_fd, (struct sockaddr *)&server_addr, &addr_len);
        if (new_sock < 0)
        {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        //Add client to the list
        pthread_mutex_lock(&clients_mutex);
        int added = 0;
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i] == 0)
            {
                clients[i] = new_sock;
                added = 1;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        if (!added)
        {
            char *msg = "Server is full. Try again later.\n";
            send(new_sock, msg, strlen(msg), 0);
            close(new_sock);
            continue;
        }

        //Create a thread to handle the client
        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = new_sock;
        pthread_create(&tid, NULL, handle_client, (void *)pclient);
    }

}

int main()
{
    printf("Hello, World!\n");
    initialize_server();
    server_running();
    return 0;
}
