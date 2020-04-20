//
// Created by ves on 20/04/2020.
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

    int result = getaddrinfo("2472e337.eu.ngrok.io", "5000", &hints, &infoptr);
    if(result) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        return;
    }

    for(p = infoptr; p != NULL; p =p->ai_next)
    {
        socket_desc = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if(socket_desc == -1)
            continue;

        if(connect(socket_desc, p->ai_addr, p->ai_addrlen) != -1) {
            puts("Connected");
            break;      /* Success */
        }

        close(socket_desc);
    }

    if(p == NULL){
        fprintf(stderr, "Could not connect\n");
        return;
    }

    freeaddrinfo(infoptr);
    char *m = "Hello, World!\n";
    if( send(socket_desc, m, strlen(m), 0) < 0){
        fprintf(stderr, "Send failed\n");
        return;
    }
    puts("Sent!");
    char rec[200];
    if( recv(socket_desc, rec, 200, 0) < 0)
        fprintf(stderr, "Recv failed\n");
    puts("Reply received.");
    puts(rec);

    close(socket_desc);
}