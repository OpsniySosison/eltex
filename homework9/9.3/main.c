#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _GNU_SOURCE

#include <unistd.h>

#define RESET   "\033[0m"
#define WHITE   "\033[1;37;40m"
#define GREEN   "\033[1;32;40m"

char* sgets(char* buff, unsigned int size) {

    int i, k = getchar();

    if (k == EOF)
        return NULL;

    for (i = 0; k != EOF && k != '\n' && i < size; i++) {
        buff[i] = k;
        k = getchar();

        if (k == EOF && !feof(stdin))
            return NULL;
    }

    buff[i] = '\0';

    return buff;
}

int main(void) {
    printf("%sWelcome to asd (command interpreter created by asd v0.1 alpha)%s\n", GREEN, RESET);

    char input[64];
    char dir[32] = "/bin/";
    char* args[32];
    char* lexeme;

    while (1) {
        printf("%sasd%s: ", WHITE, RESET);
        sgets(input, 64);
        lexeme = strtok(input, " ");
        for (unsigned int i = 0; lexeme; i++) {
            args[i] = lexeme;
            lexeme = strtok(NULL, " ");
        }
        pid_t pid = fork();
        if (pid) {
            wait();
        }
        else {
            if (execvp(strcat(dir, args[0]), args) == -1) {
                printf("File not found\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    exit(EXIT_SUCCESS);
}