
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 51234

int main() {
    char buff[20];

    int socket_d = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_d < 0) {
        fprintf(stderr, "socket not created\n");
    }

    struct sockaddr_in server;
    struct sockaddr_in client;

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(socket_d, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stderr, "bind error\n");
    }

    fprintf(stdout, "--server ready--\nport: %d\n\n", ntohs(server.sin_port));

    socklen_t c_size = sizeof(client);
    if (recvfrom(socket_d, buff, sizeof(buff), 0, (struct sockaddr*)&client, &c_size) < 0) {
        fprintf(stderr, "recvfrom error\n");
    }
    fprintf(stdout, "You get str from client: '%s'\n", buff);
    strcpy(buff, "hello");
    if (sendto(socket_d, buff, sizeof(buff), 0, (struct sockaddr*)&client, c_size) < 0) {
        fprintf(stderr, "sendto error\n");
    }
    fprintf(stdout, "you send to client str: '%s'\n", buff);

    exit(EXIT_SUCCESS);
}