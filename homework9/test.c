#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t pid = getpid();
    printf("this child process (in child func), he has a pid %d\n", pid);
    exit(EXIT_SUCCESS);
}