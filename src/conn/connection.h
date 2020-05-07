//
// Created by ves on 20/04/2020.
//

#ifndef BOMBERMAN_CLIENT_CONNECTION_H
#define BOMBERMAN_CLIENT_CONNECTION_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct connection_manager_struct{
    int socket;
    int connectionEstablished;
    int closeConnection;
    struct addrinfo hints;
    struct addrinfo *infoptr;
    struct addrinfo *p;
    char* name;
    char* port;
    pthread_t thread;
}Connection;

void initConnection(Connection* conn, char* name, char* port);

void connectServer(Connection* conn);
void* communication(void* args);

void sendName(Connection* conn);
void sendPlayerData(Connection* conn, int x, int y, unsigned int* action_counter);

void closeConnection(Connection* conn);
void closeSocket(Connection* conn);

// add argument to pass port number for TCP connection
//void test_connection();

#endif //BOMBERMAN_CLIENT_CONNECTION_H
