#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

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

char** ParseParamStr(char* str) {
    char** args = (char**)malloc(sizeof(char*));
    char* lexeme = strtok(str, " ");
    unsigned int i = 0;

    for (unsigned int i = 0; lexeme; i++) {
        args = (char**)realloc(args, sizeof(char*) * (i + 1));
        args[i] = (char*)malloc(sizeof(char) * strlen(lexeme));
        strcpy(args[i], lexeme);
        lexeme = strtok(NULL, " ");
    }

    return args;
}