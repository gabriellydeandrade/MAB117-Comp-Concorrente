#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2
#define TAMVETOR 11

typedef struct {
    int vetor[TAMVETOR];
    int posicaoInicialVetor;
    int posicaoFinalVetor;
} threadArgs;

void *threadIncremento(void * arg) {
    // Incrementar +1 no vetor

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

    int quociente = TAMVETOR / NTHREADS;
    int resto = TAMVETOR % NTHREADS;

    printf("Quantidade de tasks nas threads: %d e %d \n", quociente, quociente+resto);

    threadArgs *arg;

    pthread_t threadID[NTHREADS];
    for (int i=0; i<NTHREADS; i++){

        arg = malloc(sizeof(threadArgs));
        arg->vetor = {1,2, 3};
        arg->posicaoInicialVetor = 1;

        printf("Valor da struct %d ", arg->vetor);

        pthread_create(&threadID[i], NULL, threadIncremento, NULL);
    }

    pthread_exit(NULL);

}