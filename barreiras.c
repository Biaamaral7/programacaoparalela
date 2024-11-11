#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 3

pthread_barrier_t barrier;

void* thread_barreira(void* arg) {
    int id = *((int*)arg);
    printf("Thread %d: Começando\n", id);

    pthread_barrier_wait(&barrier);

    printf("Thread %d: Passou pela barreira\n", id);
    
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];
    
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_barreira, &ids[i]) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}
