
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    char buff[32];

    if (mkfifo("dont_delete_me", 0660) == -1) {
        if (errno != EEXIST) {
            perror("file not created "); 
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "Opening...\n");
    int fd = open("dont_delete_me", O_RDONLY);
    read(fd, buff, 32);
    fprintf(stdout, "You get '%s' from main process\n", buff);
    getchar();

    exit(EXIT_SUCCESS);
}