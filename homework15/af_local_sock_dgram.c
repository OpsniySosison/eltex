
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() { 
    char msg[6] = "hello";
    char cb[5];
    struct sockaddr_un server;
    struct sockaddr_un client;
    server.sun_family = AF_LOCAL;

    strcpy(server.sun_path, "/tmp/dgram_serv");

    int fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    if (fd < 0) {
        fprintf(stdout, "socket error\n");
    }
    
    if (bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stdout, "bind error\n");
    }

    socklen_t c_size = sizeof(client);
    if (recvfrom(fd, cb, sizeof(cb), 0, (struct sockaddr*)&client, &c_size) < 0) {
        fprintf(stderr, "recvfrom error\n");
    }
    if (sendto(fd, msg, sizeof(msg), 0, (struct sockaddr*)&client, c_size) < 0) {
        fprintf(stderr, "sendto error\n");
    }

    fprintf(stdout, "you get answer: '%s'\n", cb);
    close(fd);
    exit(EXIT_SUCCESS);
}