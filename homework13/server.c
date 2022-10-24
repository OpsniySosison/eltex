
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <mqueue.h>

#include "client.h"
#include "server.h"

int main() {

    printf("%o\n", 0777 & ~(0600));

    struct RoomList* root;
    

    mqd_t rst_mq = mq_open("/rst_mq_server", O_RDONLY | O_CREAT, 0666, );
    if (rst_mq == -1) {
        perror("rts_mq create error");
        exit(EXIT_FAILURE);
    }
    mqd_t msg_mq = mq_open("/msg_mq_server", O_RDONLY | O_CREAT);
    if (msg_mq == -1) {
        perror("rts_mq create error");
        exit(EXIT_FAILURE);
    }
    mqd_t asr_mq = mq_open("/asr_mq_client", O_WRONLY | O_CREAT);
    if (asr_mq == -1) {
        perror("rts_mq create error");
        exit(EXIT_FAILURE);
    }



    exit(EXIT_SUCCESS);
}