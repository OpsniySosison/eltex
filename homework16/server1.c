
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#define SERVER_PORT 56743
#define SERVER_NUM_CLIENT 5

static struct Args {
    struct sockaddr_in* client;
    socklen_t* size;
    int* descriptor;
    pthread_t* tid;
};

void Service(struct Args*);

static int client_count;
static pthread_spinlock_t spin;

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
    if (listen(server_socket, SERVER_NUM_CLIENT) < 0) {
        fprintf(stderr, "failed to set the number of clients\n");
    } 
    
    printf("server start on port: %d\n", SERVER_PORT);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    struct Args* args = NULL;
    client_count = 0;

    while (1) { 
        if (client_count > SERVER_NUM_CLIENT - 1) {
            fprintf(stderr, "maximum number of clients on the server\n");
            continue;
        }
        args = (struct Args*)malloc(sizeof(struct Args));
        args->descriptor = (int*)malloc(sizeof(int));
        args->client = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        args->size = (socklen_t*)malloc(sizeof(socklen_t));
        *(args->size) = sizeof(struct sockaddr_in);
        *(args->descriptor) = accept(server_socket, (struct sockaddr*)args->client, args->size);
        if (*(args->descriptor) < 0) {
            fprintf(stderr, "Failed to accept client\n");
            free(args->descriptor);
            free(args->client);
            free(args->size);
            free(args);
            continue;
        }
        args->tid = (pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(args->tid, &attr, Service, (void*)args);
        client_count++;
    }

    exit(EXIT_SUCCESS);
}

void Service(struct Args* args) {
    char msg[] = "The caption doesn't make sense";
    if (send(*(args->descriptor), msg, sizeof(msg), 0) < 0) {
        fprintf(stderr, "failed send message to client (cd %d, tid %ld)\n", *(args->descriptor), pthread_self());
    }
    else {
        fprintf(stderr, "message succesfully sended to client (cd %d, tid %ld)\n", *(args->descriptor), pthread_self());
    }
    pthread_spin_lock(&spin);
    client_count--;
    pthread_spin_unlock(&spin);
    close(*(args->descriptor));
    free(args->descriptor);
    free(args->client);
    free(args->size);
    free(args->tid);
    free(args);
    pthread_exit(NULL);
}