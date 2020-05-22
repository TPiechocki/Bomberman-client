//
// Created by Kacper on 20/04/2020.
//

#include "connection.h"

void initConnection(char *name, char *port) {
    conn = (Connection*)malloc(sizeof(Connection));

    memset(&conn->hints, 0, sizeof(conn->hints));
    conn->hints.ai_family = AF_INET;
    conn->hints.ai_socktype = SOCK_STREAM;
    conn->hints.ai_flags = 0;
    conn->hints.ai_protocol = 0;

    conn->name = name;
    conn->port = port;
    conn->closeConnection = 0;
    conn->connectionEstablished = 0;
    conn->player_count = 1;
}

void connectServer() {
    pthread_create(&conn->thread, NULL, communication, NULL);
}

void *communication(void *args) {
    // get possible IPs of server
    while (conn->closeConnection == 0) {
        int result = getaddrinfo("2.tcp.eu.ngrok.io", conn->port, &conn->hints, &conn->infoptr);
        if (result) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        }

        for (conn->p = conn->infoptr; conn->p != NULL; conn->p = conn->p->ai_next) {
            conn->socket = socket(conn->p->ai_family, conn->p->ai_socktype, conn->p->ai_protocol);

            if (conn->socket == -1)
                continue;

            // trying to connect to consecutive possible addresses
            if (connect(conn->socket, conn->p->ai_addr, conn->p->ai_addrlen) != -1) {
                puts("Connected\n");
                break;      // Success
            }

            close(conn->socket);
        }
        // none of possible addresses connected
        if (conn->p == NULL) {
            fprintf(stderr, "Could not find the server!\n");
        }
        else {

            conn->connectionEstablished = 1;
            fprintf(stdout, "Connection stable.\n");

            sendName(conn);

            char buffer[1024];
            // receive info from socket
            while (recv(conn->socket, buffer, sizeof(buffer), 0) > 0 && conn->closeConnection == 0) {
                printf("%s\n", buffer);
                // decode message from server
                decodeMessage(buffer);
                memset(buffer, 0, sizeof(buffer));
            }
        }
        conn->connectionEstablished = 0;
        if(conn->closeConnection == 0) {
            fprintf(stderr, "Connection lost, attempting reconnect...\n");
            close(conn->socket);
            sleep(5);
        }
    }
    fprintf(stdout, "Connection to server closed.");
    // close(conn->socket);
    return NULL;
}

void decodeMessage(char *message) {
    int msg, buff_length;
    char* buff_ptr = message;
    sscanf(buff_ptr, "%d%n", &msg, &buff_length);
    buff_ptr += buff_length;
    switch (msg) {
        case start_msg: {
            int enemy_c = 0;
            sscanf(buff_ptr, "%d\n%n", &conn->player_count, &buff_length);
            buff_ptr += buff_length;
            for (int i = 0; i < conn->player_count; i++) {
                char name[100];
                int x, y;
                sscanf(buff_ptr, "%s %d %d\n%n", name, &x, &y, &buff_length);
                if (strcmp(name, conn->name) == 0) {
                    // place player on proper space
                    initPlayer(board, board->start_x + board->tile_length / 2, board->start_y + board->tile_length / 2);
                    loadPlayer(window->gWindow, window->gRenderer);
                }
                else {
                    initEnemy(enemies[enemy_c], board, x, y, name);
                    loadEnemy(window->gRenderer, enemies[enemy_c]);
                    enemy_c++;
                }
                buff_ptr += buff_length;
            }
            board->startGame = 1;
        }
        break;
        case players_msg:
        {
            int playerc;
            sscanf(buff_ptr, "%d\n%n", &playerc, &buff_length);
            buff_ptr += buff_length;
            while(*buff_ptr){
                char name[100];
                int x, y;
                sscanf(buff_ptr, "%s %d %d\n%n", name, &x, &y, &buff_length);
                if(strcmp(conn->name, name) == 0){
                    buff_ptr += buff_length;
                    continue;
                }
                for(int i = 0; i < conn->player_count - 1; i++){
                    if(strcmp(name, enemies[i]->name) == 0){
                        pthread_mutex_lock(&enemy_lock);
                        enemies[i]->nextX = x;
                        enemies[i]->nextY = y;
                        enemies[i]->stepX = (enemies[i]->nextX - enemies[i]->x) / 6;
                        enemies[i]->stepY = (enemies[i]->nextY - enemies[i]->y) / 6;
                        enemies[i]->stepCounter = 0;
                        pthread_mutex_unlock(&enemy_lock);
                    }
                }
                buff_ptr += buff_length;
            }
        }
        break;
        default: break;
    }
}

void sendName() {
    if (conn->connectionEstablished == 1) {
        char buffer[100];
        sprintf(buffer, "%d %s", name_msg, conn->name);
        send(conn->socket, buffer, strlen(buffer), 0);
    }
}

void sendPlayerData(int x, int y, unsigned int *action_counter) {
    if (conn->connectionEstablished == 1) {
        // convert data to ANSI string
        char buffer[100];
        sprintf(buffer, "%d %s %d %d %d\n", move_msg, conn->name, x, y, (*action_counter)++);
        send(conn->socket, buffer, strlen(buffer), 0);
    }
}

void sendBombEvent(int tile) {
    if(conn->connectionEstablished == 1){
        char buffer[100];
        sprintf(buffer, "%d %s %d", bomb_msg, conn->name, tile);
        send(conn->socket, buffer, strlen(buffer), 0);
    }
}


void closeConnection() {
    conn->closeConnection = 1;
    close(conn->socket);
    pthread_join(conn->thread, NULL);
}

void closeSocket() {
    freeaddrinfo(conn->infoptr);
    free(conn);
}
