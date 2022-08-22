#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

unsigned long a;

void Add(void) {
    for (unsigned int i = 0; i < 1000000; i++) {
        int temp = a;
        temp++;
        a = temp;
    }
    printf("a %ld\n", a);
}

int main(void) {
    a = 0;

    pthread_t tid[5];
    pthread_attr_t attr;
    int* s;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (unsigned int i = 0 ; i < 5; i++) {
        pthread_create(&tid[i], &attr, Add, NULL);
    }

    for (unsigned int i = 0 ; i < 5; i++) {
       pthread_join(tid[i], &s);
    }

    printf("finnaly %ld\n", a);

    exit(EXIT_SUCCESS);
}