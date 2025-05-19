#ifndef UTIL_H
#define UTIL_H

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

extern Connection connections[MAX_CONNECTIONS];
extern int connection_count;
extern pthread_mutex_t connection_mutex;
extern int server_socket;

void* handle_connection(void* arg);
void* accept_connections(void* arg);
char* get_ip_address();
void help();
void myip();
void myport(int port);
void list_connections();
void connect_to_peer(char* ip, int port);
void terminate_connection(int id);
void send_message(int id, char* message);
void exit_program();

#endif // UTIL_H