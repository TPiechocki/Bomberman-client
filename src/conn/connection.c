//
// Created by Kacper on 20/04/2020.
//

#include "connection.h"

void initConnection(Connection *conn, char *name, char *port) {
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

void connectServer(Connection *conn) {
    pthread_create(&conn->thread, NULL, communication, conn);
}

void *communication(void *args) {
    Connection *conn = (Connection *) args;
    // get possible IPs of server
    while (conn->closeConnection == 0) {
        int result = getaddrinfo("localhost", conn->port, &conn->hints, &conn->infoptr);
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
                memset(buffer, 0, sizeof(buffer));
                // decode message from server
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

}

void sendName(Connection *conn) {
    if (conn->connectionEstablished == 1) {
        char buffer[100];
        sprintf(buffer, "%d %s", name, conn->name);
        send(conn->socket, buffer, strlen(buffer), 0);
    }
}

void sendPlayerData(Connection *conn, int x, int y, unsigned int *action_counter) {
    if (conn->connectionEstablished == 1) {
        // convert data to ANSI string
        char buffer[100];
        sprintf(buffer, "%s %d %d %d\n", conn->name, x, y, (*action_counter)++);
        char buffer2[100];
        sprintf(buffer2, "%d %s", /*move,*/ strlen(buffer), buffer);
        send(conn->socket, buffer2, strlen(buffer2), 0);
    }
}

void sendBombEvent(Connection *conn, int tile) {
    if(conn->connectionEstablished == 1){
        char buffer[100];
        sprintf(buffer, "%s %d", conn->name, tile);
        char buffer2[100];
        sprintf(buffer2, "%d %d %s", bomb, strlen(buffer2), buffer2);
        send(conn->socket, buffer2, strlen(buffer2), 0);
    }
}


void closeConnection(Connection *conn) {
    conn->closeConnection = 1;
    pthread_join(conn->thread, NULL);
}

void closeSocket(Connection *conn) {
    freeaddrinfo(conn->infoptr);
}
