#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

sem_t semaforo;
int count = 0;

void* thread_semaforo(void* arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&semaforo);
        count++;
        sem_post(&semaforo);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    if (sem_init(&semaforo, 0, 1) != 0) {
        perror("Erro ao inicializar semáforo");
        exit(1);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_semaforo, NULL) != 0) {
            perror("Erro ao criar thread");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Valor final do contador: %d\n", count);

    sem_destroy(&semaforo);

    return 0;
}
