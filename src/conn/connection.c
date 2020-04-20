//
// Created by Kacper on 20/04/2020.
//

#include "connection.h"

void test_connection(){
    int socket_desc;
    struct addrinfo hints, *infoptr, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype =SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    /*struct sockaddr_in server;
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("87.207.30.151");
    server.sin_family = AF_INET;
    server.sin_port = htons( 5000 );*/


    int result = getaddrinfo("0.tcp.eu.ngrok.io", "11820", &hints, &infoptr);
    if(result) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        return;
    }

    /*
    char host[256];
    for (p = infoptr; p != NULL; p = p->ai_next) {

        getnameinfo(p->ai_addr, p->ai_addrlen, host, sizeof (host), NULL, 0, NI_NUMERICHOST);
        puts(host);
    }*/

    for(p = infoptr; p != NULL; p =p->ai_next)
    {
        socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if(socket_desc == -1)
            continue;

        if(connect(socket_desc, p->ai_addr, p->ai_addrlen) != -1) {
            puts("Connected");
            break;      // Success
        }

        close(socket_desc);
    }

    if(p == NULL){
        fprintf(stderr, "Could not connect\n");
        return;
    }

    freeaddrinfo(infoptr);

    /*if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return;
    }

    puts("Connected");*/

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

    close(socket_desc);
}