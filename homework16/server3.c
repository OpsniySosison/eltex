
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pthread.h>

#define SERVER_PORT 56746
#define THREAD_POOL 3

static struct Data {
    int descriptor;
};

static struct Queue {
    struct Data* data;
    struct Queue* next;
};

static struct QueueHeadTail {
    struct Queue* head;
    struct Queue* tail;
};

static void Service(struct QueueHeadTail*);

static pthread_spinlock_t spin;

int main(void) {

    pthread_spin_init(&spin, PTHREAD_PROCESS_SHARED);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct QueueHeadTail* root = (struct QueueHeadTail*)malloc(sizeof(struct QueueHeadTail));
    root->head = NULL;
    root->tail = NULL;

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

    pthread_t th[THREAD_POOL];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    for (unsigned int i = 0; i < THREAD_POOL; i++) {
        pthread_create(&th[i], &attr, Service, (void*)root);
    }

    struct sockaddr_in c_temp;
    socklen_t c_size = sizeof(c_temp);

    printf("server start on port: %d\n", SERVER_PORT);

    while(1) { 
        int descriptor = accept(server_socket, (struct sockaddr*)&c_temp, &c_size);
        if (descriptor < 0) {
            fprintf(stderr, "unknow error %d\n", descriptor);
            continue;
        }
        pthread_spin_lock(&spin); // да поможет мне господь бог
        if (!root->head) {
            root->head = (struct Queue*)malloc(sizeof(struct Queue));
            root->head->data = (struct Data*)malloc(sizeof(struct Data));
            root->head->data->descriptor = descriptor;
            root->tail = root->head;
            pthread_spin_unlock(&spin);
            continue;
        }
        else if (!root->head->next) {
            root->head->next = (struct Queue*)malloc(sizeof(struct Queue));
            root->head->next->data = (struct Data*)malloc(sizeof(struct Data));
            root->head->next->data->descriptor = descriptor;
            if (root->tail) {
                free(root->tail);
            }
            root->tail = root->head->next;
            pthread_spin_unlock(&spin);
            continue;
        }
        pthread_spin_unlock(&spin);
        root->tail->next = (struct Queue*)malloc(sizeof(struct Queue));
        root->tail = root->tail->next;
        root->tail->data = (struct Data*)malloc(sizeof(struct Data));
        root->tail->data->descriptor = descriptor;
    }

    exit(EXIT_SUCCESS);
}

static void Service(struct QueueHeadTail* root) {
    struct Queue* temp = NULL;
    while (1) {
        pthread_spin_lock(&spin);
        if (root->head) {
            if (!root->head->data) {
                pthread_spin_unlock(&spin);
                sleep(2);
                continue;
            }
            temp = root->head;
            if (root->head->next) {
                root->head = root->head->next;
            }
            else {
                root->head = NULL;
            }
        }
        else {
            pthread_spin_unlock(&spin);
            sleep(2);
            continue;
        }
        pthread_spin_unlock(&spin);
        char msg[] = "The caption doesn't make sense";
        if (send(temp->data->descriptor, msg, sizeof(msg), 0) < 0) {
            fprintf(stderr, "failed send message to client (cd %d, tid %ld)\n", 1, pthread_self());
        }
        else {
            fprintf(stdout, "message succesfully sended to client (cd %d, tid %ld)\n", 1, pthread_self());
        }
        close(temp->data->descriptor);
        free(temp);
        temp = NULL;
        sleep(10);
    }
}