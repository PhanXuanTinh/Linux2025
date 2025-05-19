#include "../inc/util.h"

// Global variable definitions
Connection connections[MAX_CONNECTIONS];
int connection_count = 0;
pthread_mutex_t connection_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_socket;

void help() 
{
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

char* get_ip_address() 
{
    struct ifaddrs* ifaddr;
    struct ifaddrs* ifa;
    char* ip = NULL;

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        return NULL;
    }

    // Allocate memory for IP string
    ip = malloc(INET_ADDRSTRLEN);
    if (ip == NULL) 
    {
        freeifaddrs(ifaddr);
        return NULL;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_INET)
            continue;

        // Skip loopback interface
        if (strcmp(ifa->ifa_name, "lo") == 0)
            continue;

        struct sockaddr_in* sa = (struct sockaddr_in*)ifa->ifa_addr;
        if (inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN) != NULL) 
        {
            if (strcmp(ip, "127.0.0.1") != 0) 
            {
                freeifaddrs(ifaddr);
                return ip;
            }
        }
    }

    // If no valid IP found
    free(ip);
    freeifaddrs(ifaddr);
    return NULL;
}

void myip() 
{
    char* ip = get_ip_address();
    if (ip) 
    {
        printf("IP address: %s\n", ip);
        free(ip);
    } 
    else 
    {
        printf("Failed to get IP address\n");
    }
}

void myport(int port) 
{
    printf("Port: %d\n", port);
}

void list_connections() 
{
    pthread_mutex_lock(&connection_mutex);
    if (connection_count == 0) 
    {
        printf("No active connections\n");
    } 
    else 
    {
        printf("Active connections:\n");
        for (int i = 0; i < connection_count; i++) 
        {
            if (connections[i].socket != -1) 
            {
                printf("%d: %s %d\n", connections[i].id, connections[i].ip, connections[i].port);
            }
        }
    }
    pthread_mutex_unlock(&connection_mutex);
}

void* handle_connection(void* arg) 
{
    int socket = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    while (1) 
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) 
        {
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
    for (int i = 0; i < connection_count; i++) 
    {
        if (connections[i].socket == socket) 
        {
            connections[i].socket = -1;
            close(socket);
            connection_count--;
            break;
        }
    }
    pthread_mutex_unlock(&connection_mutex);
    return NULL;
}

void* accept_connections(void* arg) 
{
    int server_socket = *(int*)arg;
    free(arg);

    while (1) 
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) 
        {
            continue;
        }

        pthread_mutex_lock(&connection_mutex);
        if (connection_count >= MAX_CONNECTIONS) 
        {
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

void connect_to_peer(char* ip, int port) 
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) 
    {
        printf("Failed to create socket\n");
        return;
    }

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        printf("Failed to connect to %s:%d\n", ip, port);
        close(socket_fd);
        return;
    }

    pthread_mutex_lock(&connection_mutex);
    if (connection_count >= MAX_CONNECTIONS)
    {
        close(socket_fd);
        pthread_mutex_unlock(&connection_mutex);
        printf("Maximum connections reached\n");
        return;
    }

    struct hostent* he = gethostbyname(ip);
    if (he == NULL) 
    {
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

void terminate_connection(int id)
{
    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) 
    {
        if (connections[i].id == id) {
            if (connections[i].socket != -1) 
            {
                close(connections[i].socket);
                connections[i].socket = -1;
                connection_count--;
                printf("Connection %d terminated\n", id);
            }
            else 
            {
                printf("Connection %d not found\n", id);
            }
            break;
        }
    }
    pthread_mutex_unlock(&connection_mutex);
}

void send_message(int id, char* message) 
{
    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) 
    {
        if (connections[i].id == id) 
        {
            if (connections[i].socket != -1) 
            {
                send(connections[i].socket, message, strlen(message), 0);
                printf("Message sent to connection %d\n", id);
            }
            else 
            {
                printf("Connection %d not found\n", id);
            }
            break;
        }
    }
    pthread_mutex_unlock(&connection_mutex);
}

void exit_program() 
{
    pthread_mutex_lock(&connection_mutex);
    for (int i = 0; i < connection_count; i++) 
    {
        if (connections[i].socket != -1) 
        {
            close(connections[i].socket);
            connections[i].socket = -1;
        }
    }

    connection_count = 0;
    pthread_mutex_unlock(&connection_mutex);
    close(server_socket);
    printf("Exiting program\n");
}

