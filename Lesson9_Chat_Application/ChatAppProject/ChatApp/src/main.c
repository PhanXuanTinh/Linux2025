#include "../inc/main.h"

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    if (port < 1 || port > 65535) 
    {
        printf("Invalid port number\n");
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) 
    {
        printf("Failed to create socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        printf("Failed to bind socket\n");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 3) < 0) 
    {
        printf("Failed to listen on socket\n");
        close(server_socket);
        return 1;
    }

    printf("Server listening on port %d\n", port);

    int* server_socket_ptr = malloc(sizeof(int));
    *server_socket_ptr = server_socket;
    pthread_t accept_thread;
    pthread_create(&accept_thread, NULL, accept_connections, server_socket_ptr);

    char input[BUFFER_SIZE];
    while (1) 
    {
        printf("chat> ");
        fgets(input, BUFFER_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "help") == 0) 
        {
            help();
        }
        else if (strcmp(input, "myip") == 0) 
        {
            myip();
        } 
        else if (strcmp(input, "myport") == 0) 
        {
            myport(port);
        } 
        else if (strncmp(input, "connect ", 8) == 0) 
        {
            char* args = input + 8;
            char* ip = strtok(args, " ");
            char* port_str = strtok(NULL, " ");
            if (ip == NULL || port_str == NULL) 
            {
                printf("Usage: connect <ip> <port>\n");
                continue;
            }
            int peer_port = atoi(port_str);
            connect_to_peer(ip, peer_port);
        } 
        else if (strcmp(input, "list") == 0) 
        {
            list_connections();
        } 
        else if (strncmp(input, "terminate ", 10) == 0) 
        {
            char* id_str = input + 10;
            int id = atoi(id_str);
            terminate_connection(id);
        } 
        else if (strncmp(input, "send ", 5) == 0) 
        {
            char* args = input + 5;
            char* id_str = strtok(args, " ");
            char* message = strtok(NULL, "\0");
            if (id_str == NULL || message == NULL) 
            {
                printf("Usage: send <id> <message>\n");
                continue;
            }
            int id = atoi(id_str);
            send_message(id, message);
        } 
        else if (strcmp(input, "exit") == 0) 
        {
            exit_program();
            break;
        } 
        else 
        {
            printf("Unknown command. Type 'help' for available commands\n");
        }
    }

    return 0;
}
