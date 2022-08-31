
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    char msg[5] = "hi";
    char cb[6];
    struct sockaddr_un server;
    server.sun_family = AF_LOCAL;
    strcpy(server.sun_path, "/tmp/stream_serv");
    int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    connect(fd, (struct sockaddr*)&server, sizeof(server));
    recv(fd, cb, sizeof(cb), 0);
    send(fd, msg, sizeof(msg), 0);

    fprintf(stdout, "you get answer: '%s'\n", cb);
    close(fd);
    exit(EXIT_SUCCESS);
}