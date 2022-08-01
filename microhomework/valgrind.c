#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main() {

    int* p;

    p = (int*)malloc(sizeof(int) * 40);
    //p = (int*)malloc(sizeof(int) * 40);

    p[41] = 10;

    free(p);


    return 0;
}