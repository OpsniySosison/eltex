
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

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

int main() {
    char buff[32];
    
    sgets(buff, 32);

    if (mkfifo("dont_delete_me", 0660) == -1) {
        if (errno != EEXIST) {
            perror("file not created "); 
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "Opening...\n");
    int fd = open("dont_delete_me", O_WRONLY);
    write(fd, buff, 32);
    fprintf(stdout, "You send '%s' to other process\n", buff);
    getchar();

    exit(EXIT_SUCCESS);
}
