#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    if (pid) {
        printf("this parent process, he has a pid %d\n", pid);
        wait();
        exit(EXIT_SUCCESS);
    }
    else if (!pid) {
        printf("this child process, he has a pid %d\n", pid);
        exit(EXIT_SUCCESS);
    } 
    else {
        printf("unknow error\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}