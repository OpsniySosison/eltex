
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_PORT 56746

int main(void) {
    char msg[31];

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    socklen_t server_size = sizeof(server);

    if (connect(server_socket, (struct sockaddr*)&server, server_size) < 0) {
        fprintf(stderr, "failed connect to server\n");
    }
    if (recv(server_socket, msg, sizeof(msg), 0) < 0) {
        fprintf(stderr, "failed receiv from server\n");
    }
    fprintf(stdout, "You get str from server: '%s'\n", msg);

    close(server_socket);

    exit(EXIT_SUCCESS);
}