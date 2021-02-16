#include <stdio.h>
#include <pthread.h>

#define TAM_BUFFER 10

typedef struct {
    int *vetor;
    int inicio;
    int fim;
} threadArgs;

threadArgs *buffer[TAM_BUFFER];
int contador=0, entrada=0, saida=0;

pthread_mutex_t mutex;
pthread_cond_t cond_cons, cond_prod;

void insere_buffer(threadArgs *item) {
    pthread_mutex_lock(&mutex);
    printf("P quer inserir\n");
    while(contador == TAM_BUFFER) {
        printf("P bloqueou\n");
        pthread_cond_wait(&cond_prod, &mutex);
        printf("P desbloqueou\n");
    }
    buffer[entrada] = item;
    entrada = (entrada + 1) % TAM_BUFFER;
    contador++;
    printf("P inseriu\n");
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_cons);
}

threadArgs *retira_buffer(int id) {
    threadArgs *item;
    pthread_mutex_lock(&mutex);
    printf("C[%d] quer consumir\n", id);
    while(contador == 0) {
        printf("C[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_cons, &mutex);
        printf("C[%d] desbloqueou\n", id);
    }
    item = buffer[saida];
    saida = (saida + 1) % TAM_BUFFER;
    contador--;
    printf("C[%d] consumiu\n", id);
    printf("Inicio: %d, fim: %d \n", item->inicio, item->fim);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_prod);
    return item;
}