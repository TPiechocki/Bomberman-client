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
#include "../player.h"

typedef enum Messages_ids{
    move_msg,
    bomb_msg,
    name_msg,
    start_msg,
    end_msg,
    players_msg,
    bombs_msg,
    walls_msg,
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

pthread_mutex_t tick_lock;
int actualTick;

Connection* conn;

void initConnection(char* name, char* port);

void connectServer();
void* communication(void* args);
void decodeMessage(char* message);

void sendName();
void sendPlayerData(int x, int y, unsigned int* action_counter);
void sendBombEvent(int tile);


void closeConnection();
void closeSocket();

#endif //BOMBERMAN_CLIENT_CONNECTION_H
