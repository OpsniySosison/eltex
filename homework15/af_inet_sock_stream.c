
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    char buff[10];

    int socket_d = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    struct sockaddr_in client;

    server.sin_family = AF_INET;
    server.sin_port = htons(11231);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_d, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stderr, "bind error\n");
    }
    if (listen(socket_d, 1) < 0) {
        fprintf(stderr, "listen error\n");
    }

    socklen_t c_size = sizeof(client);
    int socket_c = accept(socket_d, (struct sockaddr*)&client, &c_size);
    if (recv(socket_c, buff, sizeof(buff), 0) < 0) {
        fprintf(stderr, "recvfrom error\n");
    }
    fprintf(stdout, "You get str from client: '%s'\n", buff);
    strcpy(buff, "hello");
    if (send(socket_c, buff, sizeof(buff), 0) < 0) {
        fprintf(stderr, "sendto error\n");
    }

    exit(EXIT_SUCCESS);
}