
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define SERVER_PORT 56744
#define THREAD_POOL 3

static struct Args {
    struct sockaddr_in client;
    socklen_t size;
    int descriptor;
    unsigned int num;
};

void Service(struct Args*);

static pthread_cond_t cond[THREAD_POOL];
static pthread_mutex_t mutex[THREAD_POOL];
static pthread_spinlock_t spin;
static unsigned int mask = 0b000;

int main(void) {

    pthread_spin_init(&spin, PTHREAD_PROCESS_SHARED);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        fprintf(stderr, "failed to bind the server\n");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, THREAD_POOL) < 0) {
        fprintf(stderr, "failed to set the number of clients\n");
        exit(EXIT_FAILURE);
    } 

    pthread_t* th_pool;
    struct Args* args_pool;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    th_pool = (pthread_t*)malloc(sizeof(pthread_t) * THREAD_POOL);
    args_pool = (struct Args*)malloc(sizeof(struct Args) * THREAD_POOL);

    for (unsigned int i = 0; i < THREAD_POOL; i++) {
        args_pool[i].num = i;
        pthread_create(&th_pool[i], &attr, Service, &args_pool[i]);
    }

    struct Args temp_args;

    printf("server start on port: %d\n", SERVER_PORT);

    while (1) {
        temp_args.size = sizeof(struct sockaddr_in);        
        temp_args.descriptor = accept(server_socket, &temp_args.client, &temp_args.size);
        if (temp_args.descriptor < 0) {
            fprintf(stderr, "unknow error\n");
            continue;
        }

        for (unsigned int i = 0; i < THREAD_POOL; i++) {
            if (!(mask & 1 << i)) {
                mask = mask | 1 << i;
                args_pool[i].client = temp_args.client;
                args_pool[i].size = temp_args.size;
                args_pool[i].descriptor = temp_args.descriptor;      
                pthread_cond_signal(&cond[i]);          
                break;
            }
        }
    }

    exit(EXIT_SUCCESS);
}

void Service(struct Args* args) {
    while(1) {
        fprintf(stdout, "thread %d wait signal!\n", args->num);
        pthread_cond_wait(&cond[args->num], &mutex[args->num]);
        fprintf(stdout, "thread %d wait unsleep!\n", args->num);
        char msg[] = "The caption doesn't make sense";
        if (send(args->descriptor, msg, sizeof(msg), 0) < 0) {
            fprintf(stderr, "failed send message to client (cd %d, tid %ld)\n", args->descriptor, pthread_self());
        }
        else {
            fprintf(stdout, "message succesfully sended to client (cd %d, tid %ld)\n", args->descriptor, pthread_self());
        }
        sleep(10);
        close(args->descriptor);
        pthread_spin_lock(&spin);
        mask = mask & ~(1 << args->num);
        pthread_spin_unlock(&spin);
    }
}