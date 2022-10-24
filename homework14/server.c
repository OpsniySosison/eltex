
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

extern int msg_sem;
extern int cb_sem;

extern int msg_shm;
extern int cb_shm;

int main(void) {
    struct RoomList* root = NULL;
    if (RoomListInit(&root)) {
        print_err_msg("List of room init error, exit.");
        exit(EXIT_FAILURE);
    }
    
    msg_sem = semget(MSG_SEM_ID, 1, IPC_CREAT | 0666);
    cb_sem = semget(CB_SEM_ID, 1, IPC_CREAT | 0666);
    if (msg_sem < 0 || cb_sem < 0) {
        print_err_msg("Get semaphore error");
        perror("semaphore not created");
        exit(EXIT_FAILURE);
    }

    print_srv_msg("Server start, please wait");

    msg_shm = shm_open("/msg_shm_server", O_RDONLY | O_CREAT, 0666);
    if (msg_shm < 0) {
        perror("msg shm_open error");
        exit(EXIT_FAILURE);
    }
    cb_shm = shm_open("/cb_shm_server", O_WRONLY | O_CREAT, 0666);
    if (cb_shm < 0) {
        perror("cb shm_open error");
        exit(EXIT_FAILURE);
    }

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_t msg_hd_pt_id;

    pthread_create(&msg_hd_pt_id, &attr, MsgHandler, (void*)root);

    print_srv_msg("Server started");

    pthread_join(msg_hd_pt_id, NULL);

    exit(EXIT_SUCCESS);
}