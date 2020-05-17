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
}

/*
void test_connection(){

    char *m = "Hello, World!\n";
    if( send(socket_desc, m, strlen(m), 0) < 0){
        fprintf(stderr, "Send failed\n");
        return;
    }
    puts("Sent!");
    char rec[500];
    if( recv(socket_desc, rec, 500, 0) < 0)
        fprintf(stderr, "Recv failed\n");
    puts("Reply received.");
    puts(rec);

}*/

void connectServer(Connection *conn) {
    pthread_create(&conn->thread, NULL, communication, conn);
}

void *communication(void *args) {
    Connection *conn = (Connection *) args;
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
            /*if (connect(conn->socket, conn->p->ai_addr, conn->p->ai_addrlen) < 0) {
                // connection lost, reset socket and retry
                conn->connectionEstablished = 0;
                sleep(2);
                fprintf(stderr, "Connection to server lost, attempting to reconnect...\n");
                close(conn->socket);
                if ((conn->socket = socket(conn->p->ai_family, conn->p->ai_socktype, conn->p->ai_protocol)) == -1) {
                    fprintf(stderr, "Socket creation error!");
                    // call for app shutdown or ignore
                    //return;
                }
            } else {*/
            // receive info from socket
            conn->connectionEstablished = 1;
            fprintf(stdout, "Connection stable.\n");

            //char *m = "Hello, World!\n";
            while (conn->closeConnection == 0) {
                sleep(1);
            }
        }
        conn->connectionEstablished = 0;
        if(conn->closeConnection == 0)
            fprintf(stderr, "Connection lost, attempting reconnect...\n");
        else
            fprintf(stdout, "Connection to server closed.");
    }
    close(conn->socket);
    return NULL;
}

void sendName(Connection *conn) {
    if (conn->connectionEstablished == 1) {
        char buffer[100];
        sprintf(buffer, "%s", conn->name);
        send(conn->socket, buffer, strlen(buffer), 0);
    }
}

void sendPlayerData(Connection *conn, int x, int y, unsigned int *action_counter) {
    if (conn->connectionEstablished == 1) {
        // convert data to ANSI string
        char buffer[100];
        sprintf(buffer, "%s %d %d %d\n", conn->name, x, y, (*action_counter)++);
        char buffer2[100];
        sprintf(buffer2, "%d %s", strlen(buffer), buffer);
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
