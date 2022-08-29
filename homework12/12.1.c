#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* sgets(char* buff, unsigned int size) {

    int i, k = getchar();

    if (k == EOF) {
        return NULL;
    }

    for (i = 0; k != EOF && k != '\n' && i < size; i++) {
        buff[i] = k;
        k = getchar();

        if (k == EOF && !feof(stdin)) {
            return NULL;
        }
    }

    buff[i] = '\0';

    return buff;
}

int main() {
    char buff[32];
    int fd[2];

    sgets(buff, 32);
    pipe(fd);
    printf("you send '%s' str in parent process\n", buff);
    write(fd[1], buff, 32);

    pid_t process = fork();
    if (process) {
       wait();
    }
    else {
        char other_buff[32];
        read(fd[0], other_buff, 32);
        printf("child process get '%s' str from parent process\n", other_buff);
    }

    exit(EXIT_SUCCESS);
}