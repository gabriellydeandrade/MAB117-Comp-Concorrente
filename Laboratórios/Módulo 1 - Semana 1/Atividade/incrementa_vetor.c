/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Laboratório: Implementar um programa concorrente, com duas threads (alem da thread principal),
 * para incrementar de 1 cada elemento de um vetor de N (10 < N < 100) elementos. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2

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
    int vetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
    int tamanhoVetor = sizeof(vetor) / sizeof(vetor[0]); // Divide o tamanho total do vetor pela quantidade de bits para saber a quantidade de elementos
    threadArgs *arg;
    pthread_t threadID[NTHREADS];

    if (tamanhoVetor <= 10 || tamanhoVetor >= 100) {
        printf("O tamanho do vetor deverá estar em um intervalo de (10 < N < 100) elementos \n");
        return 0;
    }

    printf("=========== VALORES INICIAIS =========== \n\n");
    for (int i=0; i < tamanhoVetor; i++){
        printf("Valor inicial do vetor na posição %d: %d \n", i, vetor[i]);
    }

    printf("\n");

    // Determina a quantidade de tasks a serem distribuídas para cada thread

    int qtdTasksThread1 = tamanhoVetor / NTHREADS;
    // int qtdTasksThread2 = qtdTasksThread1 + (tamanhoVetor % NTHREADS);
    // printf("Quantidade de tasks a serem atribuídas por threads: %d e %d \n\n", qtdTasksThread1, qtdTasksThread2);

    for (int i=0; i<NTHREADS; i++){

        arg = malloc(sizeof(threadArgs));
        if (!arg) {
            printf("ERRO; não foi possível alocar memória\n");
            exit(1);
        }

        arg->vetor = vetor;

        if (i==0){
            arg->posicaoInicialVetor = 0;
            arg->posicaoFinalVetor = qtdTasksThread1-1;
        } else {
            arg->posicaoInicialVetor = qtdTasksThread1;
            arg->posicaoFinalVetor = tamanhoVetor - 1;
        }

        int statusPthreadCreate = pthread_create(&threadID[i], NULL, threadIncremento, (void *) arg);
        if (statusPthreadCreate) {
            printf("ERRO; status code retornado no pthread_create() = %d\n", statusPthreadCreate);
            exit(-1);
        }
    }

    // Aguarda a execução de cada uma das threads

    for (int i=0; i<NTHREADS; i++){
        int statusPthreadJoin = pthread_join(threadID[i], NULL);
        if (statusPthreadJoin) {
            printf("ERRO; status code retornado no pthread_join() = %d\n", statusPthreadJoin);
            exit(-1);
        }
    }

    printf("=========== VALORES FINAIS =========== \n\n");
    for (int i=0; i < tamanhoVetor; i++){
        printf("Valor final do vetor na posição %d: %d \n", i, vetor[i]);
    }

    printf("\n");

    pthread_exit(NULL);

}