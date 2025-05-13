#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <netdb.h>

#define MAX_MESSAGE_LENGTH 100
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 256

typedef struct {
    int socket;
    char ip[INET_ADDRSTRLEN];
    int port;
    int id;
    pthread_t thread;
} Connection;

Connection connections[MAX_CONNECTIONS];
int connection_count = 0;
pthread_mutex_t connection_mutex = PTHREAD_MUTEX_INITIALIZER;
int server_socket;

void* handle_connection(void* arg);
void* accept_connections(void* arg);
char* get_ip_address();