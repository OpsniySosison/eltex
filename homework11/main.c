
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define RESET "\033[0m"
#define RED "\033[1;31;40m"
#define WHITE "\033[1;37;40m"
#define GREEN "\033[1;32;40m"
#define YELLOW "\033[1;33;40m"
#define PURPLE "\033[1;95;40m"

unsigned int shops[5];
int clients[3];
unsigned int loader;

pthread_mutex_t mutex[5];

int Checker() {
    for (unsigned int i = 0; i < 3; i++) {
        if (clients[i] > 0) {
            return 1;
        }
    }
    return 0;
}

void Loader() {
    printf("%sloader start!%s\n", GREEN, RESET);
    while (Checker()) {
        for (unsigned int i = 0; i < 5; i++) {
            if (pthread_mutex_trylock(&mutex[i])) {
                printf("%smutex lock failed in thread %ld (loader), number of mutex %d!\n%s", RED, pthread_self(), i, RESET);
                continue;
            }
            shops[i] += loader;
            printf("%sloader sleep in mutex %d, shop has a product %d!%s\n", YELLOW, i, shops[i], RESET);
            sleep(1);
            pthread_mutex_unlock(&mutex[i]);
        }  
    }
    printf("%sLoader finnaly!%s\n", GREEN, RESET);
}

void Clients(unsigned int* ptr_id) {
    unsigned int id = *(ptr_id);
    printf("%sthread %d start!%s\n", GREEN, id, RESET);
    while (clients[id] > 0) {
        for (unsigned int i = 0; i < 5; i++) {
            if (pthread_mutex_trylock(&mutex[i])) {
                printf("%smutex lock failed in thread %ld (client), number of mutex %d, client id %d!%s\n", RED, pthread_self(), i, id, RESET);
                continue;
            }
            clients[id] -= shops[i];
            shops[i] = 0;
            printf("%sthread %ld, client id %d, client need %d, sleep in mutex %d!%s\n", YELLOW, pthread_self(), id, clients[id], i, RESET);
            sleep(2);
            if (pthread_mutex_unlock(&mutex[i])) {
                printf("%smutex unlock failed in thread %ld (client), number of mutex %d, client id %d!%s\n", RED, pthread_self(), i, id, RESET);
                return;
            }
        }
    }
    printf("%sthread %d finnaly, client need %d!%s\n", GREEN, id, clients[id], RESET);
}

void Init() {
    for (unsigned int i = 0; i < 5; i++) {
        if (pthread_mutex_init(&mutex[i], NULL)) {
            printf("%smutex init error%s\n", RED, RESET);
            exit(EXIT_FAILURE);
        }
        shops[i] = rand() % 600 + 500;
    }

    for (unsigned int i = 0; i < 3; i++) {
        clients[i] = rand() % 1000 + 9500;
    }

    loader = 500;
}

void FinnalyInfo() {
    printf("%sshops - %d, %d, %d, %d, %d%s\n", PURPLE, shops[0], shops[1], shops[2], shops[3], shops[4], RESET);
    printf("%sclients - %d, %d, %d\n", PURPLE, clients[0], clients[1], clients[2], RESET);
}

int main() {

    Init();
    pthread_t tid[4];
    unsigned int id[3] = {0, 1, 2};
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (unsigned int i = 0; i < 3; i++) {
        pthread_create(&tid[i], &attr, Clients, (void*)&id[i]);
    }

    pthread_create(&tid[3], &attr, Loader, NULL);

    for (unsigned int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }
 
    FinnalyInfo();

    exit(EXIT_SUCCESS);
}