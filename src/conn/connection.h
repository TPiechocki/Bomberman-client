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
#include "../enemy.h"
#include "../bomb.h"
#include "../window.h"

typedef enum Messages_ids{
    move_msg,
    bomb_msg,
    name_msg,
    start_msg,
    end_msg,
    players_msg,
    bombs_msg,
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
    int player_count;
}Connection;



void initConnection(Connection* conn, char* name, char* port);

void connectServer(Connection* conn);
void* communication(void* args);
void decodeMessage(char* message, Connection* conn);

void sendName(Connection* conn);
void sendPlayerData(Connection* conn, int x, int y, unsigned int* action_counter);
void sendBombEvent(Connection* conn, int tile);


void closeConnection(Connection* conn);
void closeSocket(Connection* conn);

#endif //BOMBERMAN_CLIENT_CONNECTION_H
