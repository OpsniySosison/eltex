
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NORMAL 0 
#define BAD 1

struct Buffer {
    char* str;
    unsigned int size;
    unsigned int lenght;
    unsigned int* sizes_of_prev_strs;
    unsigned int num_of_str;
};

struct Cursor {
    int x;
    int y;
};

typedef struct Buffer BUFF;
typedef struct Cursor CURS;

typedef struct _Buffer TEST;

int BufferInit(FILE* file, BUFF* buff);
int UploadToFile(BUFF* buff, FILE* file);
int WriteToBuffer(CURS* cursor, BUFF* buff, char sym, unsigned int offset);
int BufferDestructor(BUFF* buff);
