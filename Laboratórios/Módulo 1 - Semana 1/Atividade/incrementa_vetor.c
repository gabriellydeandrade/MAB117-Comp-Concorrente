#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2
#define TAMVETOR 11

typedef struct {
    int *vetor;
    int posicaoInicialVetor;
    int posicaoFinalVetor;
} threadArgs;

void *threadIncremento(void * arg) {
    threadArgs *args = (threadArgs *) arg;

    for (int i=args->posicaoInicialVetor; i<=args->posicaoFinalVetor; i++){
        ++args->vetor[i];
    }

    free(arg);
    pthread_exit(NULL);
}

int main() {
    int vetor[TAMVETOR] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    printf("========= VALORES INICIAIS ========= \n\n");
    for (int i=0; i < TAMVETOR; i++){
        printf("Valor inicial do vetor na posição %d: %d \n", i, vetor[i]);
    }

    printf("\n");

    // Determina a quantidade de tasks a serem distribuídas para cada thread

    int qtdTasksThread1 = TAMVETOR / NTHREADS;
    int qtdTasksThread2 = qtdTasksThread1 + (TAMVETOR % NTHREADS);

    printf("Quantidade de tasks a serem atribuídas por threads: %d e %d \n\n", qtdTasksThread1, qtdTasksThread2);

    threadArgs *arg;

    pthread_t threadID[NTHREADS];
    for (int i=0; i<NTHREADS; i++){

        arg = malloc(sizeof(threadArgs));
        if (!arg) {
            printf("Erro ao alocar memória\n");
            exit(1);
        }

        arg->vetor = vetor;

        if (i==0){
            arg->posicaoInicialVetor = 0;
            arg->posicaoFinalVetor = qtdTasksThread1-1;
        } else {
            arg->posicaoInicialVetor = qtdTasksThread1;
            arg->posicaoFinalVetor = TAMVETOR-1;
        }

        pthread_create(&threadID[i], NULL, threadIncremento, (void *) arg);
    }

    // Aguarda a execução de cada uma das threads

    for (int i=0; i<NTHREADS; i++){
        pthread_join(threadID[i], NULL);
    }

    printf("========= VALORES FINAIS ========= \n\n");
    for (int i=0; i < TAMVETOR; i++){
        printf("Valor final do vetor na posição %d: %d \n", i, vetor[i]);
    }

    printf("\n");

    pthread_exit(NULL);

}