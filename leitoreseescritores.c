#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_LEITORES 3
#define NUM_ESCRITORES 2


int leitores = 0;                       
int escritores = 0;                     
int lido = 0;                           
pthread_mutex_t mutex;                 
pthread_cond_t condLeitores;          
pthread_cond_t condEscritores;        


void* leitor(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        while (escritores > 0) {  
            pthread_cond_wait(&condLeitores, &mutex);
        }
        leitores++;
        pthread_mutex_unlock(&mutex);

        printf("Leitor %d: valor lido: %d\n", id, lido);
        sleep(rand() % 2 + 1);  

        pthread_mutex_lock(&mutex);
        leitores--;
        if (leitores == 0) {  
            pthread_cond_signal(&condEscritores);
        }
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 3 + 1);  
    }

    return NULL;
}

void* escritor(void* arg) {
    int id = *((int*)arg);
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        while (leitores > 0 || escritores > 0) {  
            pthread_cond_wait(&condEscritores, &mutex);
        }
        escritores++;
        pthread_mutex_unlock(&mutex);

        lido = rand() % 100; 
        printf("Escritor %d: valor escrito: %d\n", id, lido);
        sleep(rand() % 3 + 1);  

        pthread_mutex_lock(&mutex);
        escritores--;
        pthread_cond_broadcast(&condLeitores);  
        pthread_cond_signal(&condEscritores);  
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 4 + 1); 
    }

    return NULL;
}

int main() {
    pthread_t threads[NUM_LEITORES + NUM_ESCRITORES];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condLeitores, NULL);
    pthread_cond_init(&condEscritores, NULL);

    for (int i = 0; i < NUM_LEITORES; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, leitor, id);
    }

    for (int i = 0; i < NUM_ESCRITORES; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[NUM_LEITORES + i], NULL, escritor, id);
    }

    for (int i = 0; i < NUM_LEITORES + NUM_ESCRITORES; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condLeitores);
    pthread_cond_destroy(&condEscritores);

    return 0;
}
