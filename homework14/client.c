
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

int main(void)
{
    system("clear");

    msg_sem = semget(MSG_SEM_ID, 0, 0);
    cb_sem = semget(CB_SEM_ID, 0, 0);

    if (msg_sem < 0 || cb_sem < 0) {
        print_err_msg("get semaphore error");
        exit(EXIT_FAILURE);
    }

    print_srv_msg("Chat by asd v0.1");

    int msg_shm = shm_open("/msg_shm_server", O_WRONLY | O_CREAT, 0666);
    if (msg_shm < 0) {
        perror("msg shm_open error");
        exit(EXIT_FAILURE);
    }
    int cb_shm = shm_open("/cb_shm_server", O_RDONLY | O_CREAT, 0666);
    if (cb_shm < 0) {
        perror("cb shm_open error");
        exit(EXIT_FAILURE);
    }

    print_srv_msg("Enter you name");
    struct Client user = {"", 0};
    sgets(&user.name, 16);

    struct Message call = {user, LARGE, LOGIN, ""};
    struct CallBackMsg cb_msg;
    while (semctl(msg_sem, 0, GETVAL));
    semctl(msg_sem, 0, SETVAL, 1);
    size_t len = lseek(msg_shm, 0, SEEK_END);
    write(msg_shm, &call, sizeof(struct Message));
    lseek(msg_shm, len, SEEK_SET);
    semctl(msg_sem, 0, SETVAL, 0);
    print_srv_msg("Waiting answer from server");
    while (!semctl(cb_sem, 0, GETVAL));
    print_srv_msg("Answer received");
    int status = read(cb_shm, &cb_msg, sizeof(struct CallBackMsg));
    if (status == -1 || !status) {
        cb_msg.num_room = 0;
        print_err_msg("Error while reading data from server");
    }
    else {
        user.id = cb_msg.user.id;
        print_srv_msg("You successfully connect to server, your id equals %d", cb_msg.user.id);
    }
    semctl(cb_sem, 0, SETVAL, 0);   

    pthread_t msg_chd_pt_id;
    pthread_attr_t attr_c;
    pthread_attr_init(&attr_c);
    pthread_attr_setdetachstate(&attr_c, PTHREAD_CREATE_JOINABLE);

    struct PthreadClientArgs msg_args = {user, msg_shm};
    struct PthreadClientArgs cb_args = {user, cb_shm};

    if (pthread_create(&msg_chd_pt_id, &attr_c, MsgClientHandler, (void*)&msg_args)) {
        print_err_msg("Create thread error");
        exit(EXIT_FAILURE);
    }

    print_srv_msg("Weclome to chat room number %d", cb_msg.num_room);

    pthread_join(msg_chd_pt_id, NULL);

    exit(EXIT_SUCCESS);
}