#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

pthread_mutex_t hashi[NUM_FILOSOFOS];

void* pensar(void* arg) {
    int id = *((int*)arg);
    int esquerda = id;                   
    int direita = (id + 1) % NUM_FILOSOFOS; 

    while (1) {
        printf("Filósofo %d está pensando\n", id);
        usleep(rand() % 1000); 

        printf("Filósofo %d tentando pegar o hashi da esquerda %d\n", id, esquerda);
        pthread_mutex_lock(&hashi[esquerda]);

        printf("Filósofo %d pegou o hashi da esquerda %d\n", id, esquerda);

        printf("Filósofo %d tentando pegar o hashi da direita %d\n", id, direita);
        pthread_mutex_lock(&hashi[direita]);

        printf("Filósofo %d pegou o hashi da direita %d e está comendo\n", id, direita);
        usleep(rand() % 1000); 

        pthread_mutex_unlock(&hashi[esquerda]);
        pthread_mutex_unlock(&hashi[direita]);

        printf("Filósofo %d terminou de comer e liberou os hashis.\n", id);
    }
    return NULL;
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        if (pthread_mutex_init(&hashi[i], NULL) != 0) {
            perror("Erro ao inicializar mutex do hashi");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        if (pthread_create(&filosofos[i], NULL, pensar, &ids[i]) != 0) {
            perror("Erro ao criar a thread do filósofo");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&hashi[i]);
    }

    return 0;
}
