
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main() {
    char msg[6] = "hello";
    char cb[5];
    struct sockaddr_un server;
    struct sockaddr_un client;
    server.sun_family = AF_LOCAL;
    strcpy(server.sun_path, "/tmp/stream_serv");
    int fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (fd < 0) {
        fprintf(stdout, "socket error\n");
    }
    if (bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stdout, "bind error\n");
    }
    if (listen(fd, 1) < 0) {
        fprintf(stdout, "listen error\n");
    }
    socklen_t size = sizeof(struct sockaddr_un);
    int cl_fd = accept(fd, (struct sockaddr*)&client, &size);
    if (cl_fd < 0) {
        fprintf(stdout, "cl_fd error\n");
    }
    if (send(cl_fd, msg, sizeof(msg), 0) < 0) {
        fprintf(stdout, "send error\n");
    }
    if (recv(cl_fd, cb, sizeof(cb), 0) < 0) {
        fprintf(stdout, "recv error\n");
    }

    fprintf(stdout, "you get answer: '%s'\n", cb);
    close(fd);
    exit(EXIT_SUCCESS);
}