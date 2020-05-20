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

typedef enum Messages_ids{
    move,
    bomb,
    name,
    end,
    start,
    failure
}MSG;

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
void decodeMessage(char* message);

void sendName(Connection* conn);
void sendPlayerData(Connection* conn, int x, int y, unsigned int* action_counter);
void sendBombEvent(Connection* conn, int tile);


void closeConnection(Connection* conn);
void closeSocket(Connection* conn);

#endif //BOMBERMAN_CLIENT_CONNECTION_H
