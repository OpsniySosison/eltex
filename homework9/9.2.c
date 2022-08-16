#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();
    if (pid) {
        printf("this parent process, he has a pid %d\n", pid);
        wait();
        exit(EXIT_SUCCESS);
    }
    else if (!pid) {
        char* args[] = {"test", NULL};
        printf("this child process (in parent func), he has a pid %d\n", pid);
        execvp("./test", args);
    } 
    else {
        printf("unknow error\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}