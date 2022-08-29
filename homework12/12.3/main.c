#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"

#define RESET  "\033[0m"
#define RED    "\033[1;31;40m"
#define WHITE  "\033[1;37;40m"
#define GREEN  "\033[1;32;40m"
#define YELLOW "\033[1;33;40m"
#define PURPLE "\033[1;95;40m"

int main(void) {
    fprintf(stdout, "%sWelcome to asd (command interpreter created by asd v0.2 alpha)\n%s", GREEN, RESET);

    char buff[64];
    char dir[] = "/bin/";
    char* lexeme;
    char** args;
    int fd[2];

    while(1) {
        fprintf(stdout, "%sasd: %s", WHITE, RESET);
        sgets(buff, 64);
        lexeme = strtok(buff, "|");
        for (unsigned int i = 0; lexeme; i++) {
            pipe(fd);
            pid_t process = fork();
            if (process) {
                wait();
                lexeme = strtok(NULL, "|");
                if (!lexeme) {
                    break;
                }
                pid_t process_1 = fork();
                if (process_1) {
                    wait();
                    lexeme = strtok(NULL, "|");
                }
                else {
                    args = ParseParamStr(lexeme);
                    dup2(fd[0], 0);
                    if (execvp(strcat(dir, args[0]), args) == -1) {
                        fprintf(stderr, "%sCommand not found!%s\n", RED, RESET);
                        exit(EXIT_FAILURE);
                    }
                }
            }
            else {
                args = ParseParamStr(lexeme);
                dup2(fd[1], 1);
                if (execvp(strcat(dir, args[0]), args) == -1) {
                    fprintf(stderr, "%sCommand not found!%s\n", RED, RESET);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    exit(EXIT_SUCCESS);
}