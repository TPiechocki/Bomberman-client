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

    while (conn->closeConnection == 0) {
        // get possible IPs of server
        /*int result = getaddrinfo("serveo.net", conn->port, &conn->hints, &conn->infoptr);
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
        }*/

        struct sockaddr_in server;
        conn->socket = socket(AF_INET , SOCK_STREAM , 0);
        if (conn->socket == -1)
        {
            printf("Could not create socket");
        }

        server.sin_addr.s_addr = inet_addr("87.207.30.151");
        server.sin_family = AF_INET;
        server.sin_port = htons( 5000 );

        if (connect(conn->socket, (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            puts("connect error");
        }
        // none of possible addresses connected
        /*if (conn->p == NULL) {
            fprintf(stderr, "Could not find the server!\n");
        }*/
        else {

            conn->connectionEstablished = 1;
            fprintf(stdout, "Connection stable.\n");

            sendName();

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
    close(conn->socket);
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
            int player_number;
            sscanf(buff_ptr, "%d\n%n", &conn->player_count, &buff_length);
            buff_ptr += buff_length;
            for (int i = 0; i < conn->player_count; i++) {
                char name[100];
                int x, y;
                sscanf(buff_ptr, "%d %s %d %d\n%n",&player_number, name, &x, &y, &buff_length);
                if (strcmp(name, conn->name) == 0) {
                    initPlayer(board, player_number, x, y, i);
                    initBomb(bombs[i]);
                    pthread_mutex_lock(&renderer_lock);
                    loadPlayer(window->gWindow, window->gRenderer);
                    loadBomb(bombs[i], window->gRenderer);
                    pthread_mutex_unlock(&renderer_lock);
                }
                else {
                    initEnemy(enemies[enemy_c], board, player_number, x, y, name, i);
                    initBomb(bombs[i]);
                    pthread_mutex_lock(&renderer_lock);
                    loadEnemy(window->gRenderer, enemies[enemy_c], enemy_c);
                    loadBomb(bombs[i], window->gRenderer);
                    pthread_mutex_unlock(&renderer_lock);
                    enemy_c++;
                }
                buff_ptr += buff_length;
            }
            board->startGame = 1;
        }
        break;
        case players_msg:
        {
            if(board->startGame == 0)
                break;
            int playerc, tick_number;
            sscanf(buff_ptr, "%d %d\n%n", &playerc, &tick_number, &buff_length);
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
                        if(x != 0 && y != 0){
                            pthread_mutex_lock(&enemy_lock);
                            enemies[i]->nextX = x;
                            enemies[i]->nextY = y;
                            enemies[i]->stepX = (enemies[i]->nextX - enemies[i]->x) / 6;
                            enemies[i]->stepY = (enemies[i]->nextY - enemies[i]->y) / 6;
                            enemies[i]->stepCounter = 0;
                            pthread_mutex_unlock(&enemy_lock);
                        }
                    }
                }
                buff_ptr += buff_length;
            }
        }
        break;
        case bombs_msg:
        {
            if(board->startGame == 0)
                break;
            int playerc, tic_number;
            sscanf(buff_ptr, "%d %d\n%n", &playerc, &tic_number, &buff_length);
            buff_ptr += buff_length;
            while(*buff_ptr){
                char name[100];
                int tile_number, explode_tick;
                sscanf(buff_ptr, "%s %d %d\n%n", name, &tile_number, &explode_tick, &buff_length);
                if(strcmp(conn->name, name) == 0){
                    buff_ptr += buff_length;
                    continue;
                }
                for(int i = 0; i < conn->player_count - 1; i++){
                    if(strcmp(name, enemies[i]->name) == 0);
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
        sprintf(buffer, "%d %s %d\n", bomb_msg, conn->name, tile);
        send(conn->socket, buffer, strlen(buffer), 0);
    }
}


void closeConnection() {
    conn->closeConnection = 1;
    //close(conn->socket);
    pthread_join(conn->thread, NULL);
}

void closeSocket() {
    // freeaddrinfo(conn->infoptr);
    free(conn);
}
