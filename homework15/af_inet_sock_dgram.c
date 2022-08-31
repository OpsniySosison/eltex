
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    char buff[10];

    int socket_d = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server;
    struct sockaddr_in client;

    server.sin_family = AF_INET;
    server.sin_port = htons(23423);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(socket_d, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stderr, "bind error\n");
    }

    socklen_t c_size = sizeof(client);
    if (recvfrom(socket_d, buff, sizeof(buff), 0, (struct sockaddr*)&client, &c_size) < 0) {
        fprintf(stderr, "recvfrom error\n");
    }
    fprintf(stdout, "You get str from client: '%s'\n", buff);
    strcpy(buff, "hello");
    if (sendto(socket_d, buff, sizeof(buff), 0, (struct sockaddr*)&client, c_size) < 0) {
        fprintf(stderr, "sendto error\n");
    }

    exit(EXIT_SUCCESS);
}