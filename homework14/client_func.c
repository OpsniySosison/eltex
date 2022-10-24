
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "client.h"
#include "server.h"

int msg_sem;
int cb_sem;

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

int MsgClientHandler(struct PthreadClientArgs* args) {
    int msg_shm = args->shm_id;
    struct Client user = args->user;
    struct Message msg;
    msg.priority = NORMAL;
    msg.type_msg = COMMUN;
    msg.user = user;
    while (1) {
        sgets(&msg.msg, 64);
        while (semctl(msg_sem, 0, GETVAL, 0));
        semctl(msg_sem, 0, SETVAL, 1);
        size_t len = lseek(msg_shm, 0, SEEK_END);
        write(msg_shm, &msg, sizeof(struct Message));
        lseek(msg_shm, len, SEEK_SET);
        semctl(msg_sem, 0, SETVAL, 0);
    }
}

int CbHandler(struct PthreadArgs* args) {

}