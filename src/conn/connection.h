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
    move_msg, // player moved
    bomb_msg, // bomb placed
    name_msg, // player name
    start_msg, // start of game message
    end_msg, // end of game message
    players_msg, // status of players
    bombs_msg, // status of bombs
    walls_msg, // status of all breakable walls
    destroy_msg, // destroyed wall
}MSG;

typedef struct connection_manager_struct{
    int socket; // socket handle
    int connectionEstablished; // status of connection; if connection is established
    int closeConnection; // prompt to close connection
    struct addrinfo hints; // structures for DNS usage - unused
    struct addrinfo *infoptr; // -||- - unused
    struct addrinfo *p; // -||- - unused
    char* name; // name of player
    char* port; // port to connect to - unused
    pthread_t thread; // thread for server connection
    int player_count; // count of players
}Connection;

pthread_mutex_t tick_lock; // tick mutex
int actualTick; // last tick recieved from server

Connection* conn; // connection handle

/**
 * Initialises connection structure and tick mutex.
 * @param name - name of player
 */
void initConnection(char* name/*, char* port*/);

/**
 * Creates new thread that connects to server.
 */
void connectServer();
/**
 * Function running in thread, receives information from server.
 * @param args - none given to thread
 * @return NULL - successful end of connection
 */
void* communication(void* args);
/**
 * Decodes messages from server and handles the results.
 * @param message - message from server
 */
void decodeMessage(char* message);

/**
 * Sends player name to server upon connection.
 */
void sendName();
/**
 * Sends current player position to server.
 * @param x - current player coordinate x
 * @param y - current player coordinate y
 * @param action_counter - player action counter
 */
void sendPlayerData(int x, int y, unsigned int* action_counter);
/**
 * Sends message to server that bomb has been placed.
 * @param tile - tile on which bomb was placed.
 */
void sendBombEvent(int tile);

/**
 * Set close connection flag and wait for thread to end.
 */
void closeConnection();
/**
 * Frees memory and destroys tick mutex.
 */
void closeConnStruct();

#endif //BOMBERMAN_CLIENT_CONNECTION_H
