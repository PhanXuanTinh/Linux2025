#include "../inc/main.h"

void help() {
    printf("Available commands:\n");
    printf("help            Display this help message\n");
    printf("myip           Display the IP address of this process\n");
    printf("myport         Display the port number of this process\n");
    printf("connect <ip> <port>  Connect to another peer\n");
    printf("list            List all active connections\n");
    printf("terminate <id>  Terminate a specific connection\n");
    printf("send <id> <message>  Send a message to a specific connection\n");
    printf("exit            Exit the program\n");
}

char* get_ip_address() {
    struct ifaddrs* ifaddr;
    struct ifaddrs* ifa;
    char* ip = NULL;

    if (getifaddrs(&ifaddr) == -1) {
        return NULL;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sa->sin_addr), ip_str, INET_ADDRSTRLEN);
            if (strcmp(ip_str, "127.0.0.1") != 0) {
                ip = strdup(ip_str);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return ip;
}

void myip() {
    char* ip = get_ip_address();
    if (ip) {
        printf("IP address: %s\n", ip);
        free(ip);
    } else {
        printf("Failed to get IP address\n");
    }
}

void myport(int port) {
    printf("Port: %d\n", port);
}

void list_connections() {
    pthread_mutex_lock(&connection_mutex);
    if (connection_count == 0) {
        printf("No active connections\n");
    } else {
        printf("Active connections:\n");
        for (int i = 0; i < connection_count; i++) {
            if (connections[i].socket != -1) {
                printf("%d: %s %d\n", connections[i].id, connections[i].ip, connections[i].port);
            }
        }
    }
    pthread_mutex_unlock(&connection_mutex);
}

void* handle_connection(void* arg) {
    int socket = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            break;
        }

        struct sockaddr_in peer_addr;
        socklen_t peer_len = sizeof(peer_addr);
        getpeername(socket, (struct sockaddr*)&peer_addr, &peer_len);
        char peer_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &peer_addr.sin_addr, peer_ip, INET_ADDRSTRLEN);
        int peer_port = ntohs(peer_addr.sin_port);

        printf("Message received from %s\n", peer_ip);
        printf("Sender's Port: %d\n", peer_port);
        printf("Message: %s\n", buffer);
    }

    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].socket == socket) {
            connections[i].socket = -1;
            close(socket);
            connection_count--;
            break;
        }
    }
    pthread_mutex_unlock(&connection_mutex);
    return NULL;
}

void* accept_connections(void* arg) {
    int server_socket = *(int*)arg;
    free(arg);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            continue;
        }

        pthread_mutex_lock(&connection_mutex);
        if (connection_count >= MAX_CONNECTIONS) {
            close(client_socket);
            pthread_mutex_unlock(&connection_mutex);
            continue;
        }

        char* client_ip = inet_ntoa(client_addr.sin_addr);
        int client_port = ntohs(client_addr.sin_port);

        int* socket_ptr = malloc(sizeof(int));
        *socket_ptr = client_socket;

        Connection* conn = &connections[connection_count];
        conn->socket = client_socket;
        strcpy(conn->ip, client_ip);
        conn->port = client_port;
        conn->id = connection_count + 1;
        pthread_create(&conn->thread, NULL, handle_connection, socket_ptr);
        connection_count++;
        pthread_mutex_unlock(&connection_mutex);

        printf("New connection from %s:%d\n", client_ip, client_port);
    }
    return NULL;
}

void connect_to_peer(char* ip, int port) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        printf("Failed to create socket\n");
        return;
    }

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to connect to %s:%d\n", ip, port);
        close(socket_fd);
        return;
    }

    pthread_mutex_lock(&connection_mutex);
    if (connection_count >= MAX_CONNECTIONS) {
        close(socket_fd);
        pthread_mutex_unlock(&connection_mutex);
        printf("Maximum connections reached\n");
        return;
    }

    struct hostent* he = gethostbyname(ip);
    if (he == NULL) {
        printf("Invalid IP address\n");
        close(socket_fd);
        pthread_mutex_unlock(&connection_mutex);
        return;
    }

    char* client_ip = inet_ntoa(*((struct in_addr*)he->h_addr_list[0]));
    int client_port = port;

    int* socket_ptr = malloc(sizeof(int));
    *socket_ptr = socket_fd;

    Connection* conn = &connections[connection_count];
    conn->socket = socket_fd;
    strcpy(conn->ip, client_ip);
    conn->port = client_port;
    conn->id = connection_count + 1;
    pthread_create(&conn->thread, NULL, handle_connection, socket_ptr);
    connection_count++;
    pthread_mutex_unlock(&connection_mutex);

    printf("Connected to %s:%d\n", client_ip, client_port);
}

void terminate_connection(int id) {
    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].id == id) {
            if (connections[i].socket != -1) {
                close(connections[i].socket);
                connections[i].socket = -1;
                connection_count--;
                printf("Connection %d terminated\n", id);
            } else {
                printf("Connection %d not found\n", id);
            }
            break;
        }
    }
    pthread_mutex_unlock(&connection_mutex);
}

void send_message(int id, char* message) {
    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].id == id) {
            if (connections[i].socket != -1) {
                send(connections[i].socket, message, strlen(message), 0);
                printf("Message sent to connection %d\n", id);
            } else {
                printf("Connection %d not found\n", id);
            }
            break;
        }
    }
    pthread_mutex_unlock(&connection_mutex);
}

void exit_program() {
    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) {
        if (connections[i].socket != -1) {
            close(connections[i].socket);
            connections[i].socket = -1;
        }
    }
    connection_count = 0;
    pthread_mutex_unlock(&connection_mutex);
    close(server_socket);
    printf("Exiting program\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    if (port < 1 || port > 65535) {
        printf("Invalid port number\n");
        return 1;
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to bind socket\n");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 3) < 0) {
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
    while (1) {
        printf("chat> ");
        fgets(input, BUFFER_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "help") == 0) {
            help();
        } else if (strcmp(input, "myip") == 0) {
            myip();
        } else if (strcmp(input, "myport") == 0) {
            myport(port);
        } else if (strncmp(input, "connect ", 8) == 0) {
            char* args = input + 8;
            char* ip = strtok(args, " ");
            char* port_str = strtok(NULL, " ");
            if (ip == NULL || port_str == NULL) {
                printf("Usage: connect <ip> <port>\n");
                continue;
            }
            int peer_port = atoi(port_str);
            connect_to_peer(ip, peer_port);
        } else if (strcmp(input, "list") == 0) {
            list_connections();
        } else if (strncmp(input, "terminate ", 10) == 0) {
            char* id_str = input + 10;
            int id = atoi(id_str);
            terminate_connection(id);
        } else if (strncmp(input, "send ", 5) == 0) {
            char* args = input + 5;
            char* id_str = strtok(args, " ");
            char* message = strtok(NULL, "\0");
            if (id_str == NULL || message == NULL) {
                printf("Usage: send <id> <message>\n");
                continue;
            }
            int id = atoi(id_str);
            send_message(id, message);
        } else if (strcmp(input, "exit") == 0) {
            exit_program();
            break;
        } else {
            printf("Unknown command. Type 'help' for available commands\n");
        }
    }

    return 0;
}
