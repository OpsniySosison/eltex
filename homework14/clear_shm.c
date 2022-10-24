
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "client.h"
#include "server.h"

int main() {
    shm_unlink("/msg_shm_server");
    shm_unlink("/cb_shm_server");
    
    exit(EXIT_SUCCESS);
}