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

    printf("argumenentos %d %d \n", args->posicaoInicialVetor, args->posicaoFinalVetor);

    for (int i=args->posicaoInicialVetor; i<=args->posicaoFinalVetor; i++){
        ++args->vetor[i];
    }

    free(arg);
    pthread_exit(NULL);
}

int main() {
    int vetor[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
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

    int qtdTasks = tamanhoVetor / NTHREADS;
    int qtdTasksExtras = tamanhoVetor % NTHREADS;
    threadArgs * vetorThreadArgs[NTHREADS];
    int posicaoInicial = 0;
    int posicaoFinal = qtdTasks;

    for (int i=0; i<NTHREADS; i++){
        arg = malloc(sizeof(threadArgs));
        if (!arg) {
            printf("ERRO; não foi possível alocar memória\n");
            exit(1);
        }

        arg->vetor = vetor;
        arg->posicaoInicialVetor = posicaoInicial;

        if (qtdTasksExtras){
            arg->posicaoFinalVetor = (qtdTasks + qtdTasksExtras) - 1;
            qtdTasksExtras = 0;
        } else {
            arg->posicaoFinalVetor = posicaoFinal - 1;
        }

        vetorThreadArgs[i] = arg;

        posicaoInicial = arg->posicaoFinalVetor + 1;
        posicaoFinal = posicaoInicial + qtdTasks;
    }


    for (int i=0; i<NTHREADS; i++){
        int statusPthreadCreate = pthread_create(&threadID[i], NULL, threadIncremento, (void *) vetorThreadArgs[i]);
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
