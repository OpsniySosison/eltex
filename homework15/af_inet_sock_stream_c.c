
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    char buff[10] = "hi";

    int socket_d = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(11231);
    server.sin_addr.s_addr = INADDR_ANY;
    
    socklen_t s_size = sizeof(server);

    if (connect(socket_d, (struct sockaddr*)&server, s_size) < 0) {
        fprintf(stderr, "connect error\n");
    }
    if (send(socket_d, buff, sizeof(buff), 0) < 0) {
        fprintf(stderr, "recvfrom error\n");
    }
    if (recv(socket_d, buff, sizeof(buff), 0) < 0) {
        fprintf(stderr, "sendto error\n");
    }
    fprintf(stdout, "You get str from server: '%s'\n", buff);

    exit(EXIT_SUCCESS);
}