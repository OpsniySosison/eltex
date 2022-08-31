
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    char msg[5] = "hi";
    char cb[6];

    struct sockaddr_un server;
    server.sun_family = AF_LOCAL;

    strcpy(server.sun_path, "/tmp/dgram_serv");

    int fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (fd < 0) {
        fprintf(stdout, "fd error\n");
    }
    if (connect(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stdout, "connect error\n");
    }
    if (send(fd, msg, sizeof(msg), 0)) {
        fprintf(stdout, "send error\n");
    }
    if (recv(fd, cb, sizeof(cb), 0) < 0) {
        fprintf(stdout, "recv error\n");
    }

    fprintf(stdout, "you get answer: '%s'\n", cb);
    close(fd);
    exit(EXIT_SUCCESS);
}