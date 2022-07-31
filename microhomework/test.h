#include <stdio.h>

#ifndef PLAYER 
#define PLAYER
    struct Player {
        int x;
        int y;
    };
#endif

#define DEBUG 1

#define dprint(fmt, ...) do {if (DEBUG) fprintf(stderr, "%s:%d:%s() "fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); } while(0)