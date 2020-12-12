/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Laboratório: Implementar um programa concorrente de multiplicação de matrizes quadradas */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

void aloca_memoria(int dimensao);
void inicializa_matriz(int dimensao);
void *executa_tarefa(void *arg);
void aguarda_execucao_das_tarefas(pthread_t *thread_ids_sistema);
void imprime_matrizes(int dimensao);
void libera_memoria();

float *matriz_1, *matriz_2, *matriz_resultante;
int dimensao_matriz, quantidade_threads;


int main(int argc, char* argv[]){

    /*
        PARÂMETROS
        Recebe a dimensão da matriz e a quantidade de threads. Por convenção estamos assumindo que a matriz é quadrada.

        DESCRIÇÃO
        Calcula a multiplicação de duas matrizes quadradas de forma concorrente.

        VALOR RETORNADO
        Matriz resultante de dimensão N recebida por parâmetro.

    */

    double tempo_inicial, tempo_final, tempo_delta;
    time_t t;

    // Inicializa a semente randômica que iremos usar na inicialização das matrizes
    srand((unsigned) time(&t));

    GET_TIME(tempo_inicial);

    if (argc < 3) {
        printf("Informe a dimensão da matriz e o número de threads do programa <%s> por argumento \n", argv[0]);
        return 1;
    }

    dimensao_matriz = atoi(argv[1]);
    quantidade_threads = atoi(argv[2]);

    pthread_t thread_ids_sistema[quantidade_threads];
    int thread_ids_local[quantidade_threads];

    if (quantidade_threads > dimensao_matriz){
        quantidade_threads = dimensao_matriz;
    }

    aloca_memoria(dimensao_matriz);

    inicializa_matriz(dimensao_matriz);

    GET_TIME(tempo_final);
    tempo_delta = tempo_final - tempo_inicial;
    printf("Tempo de inicialização: %lf\n", tempo_delta);

    GET_TIME(tempo_inicial);

    // multiplicação das matrizes (concorrência, exploração CPU Bound)

    for (int thread = 0; thread < quantidade_threads; thread++){

        thread_ids_local[thread] = thread;
        int status_pthread_create = pthread_create(&thread_ids_sistema[thread], NULL, executa_tarefa, (void *) &thread_ids_local[thread]);

        if (status_pthread_create) {
            printf("ERRO; status code retornado no pthread_create() = %d\n", status_pthread_create);
            return 2;
        }
    }

    aguarda_execucao_das_tarefas(thread_ids_sistema);

    GET_TIME(tempo_final);
    tempo_delta = tempo_final - tempo_inicial;
    printf("Tempo de multiplicação: %lf \n", tempo_delta);

    GET_TIME(tempo_inicial);

//    imprime_matrizes(dimensao_matriz);

    libera_memoria();

    GET_TIME(tempo_final);
    tempo_delta = tempo_final - tempo_inicial;
    printf("Tempo de finalização: %lf \n", tempo_delta);

    return 0;
}

void aloca_memoria(int dimensao) {
    matriz_1 = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if (!matriz_1) {
        printf("ERRO; não foi possível alocar memória para a matriz_1 \n");
        exit(1);
    }
    matriz_2 = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if (!matriz_2) {
        printf("ERRO; não foi possível alocar memória para a matriz_2 \n");
        exit(1);
    }

    matriz_resultante = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if (!matriz_resultante) {
        printf("ERRO; não foi possível alocar memória para a matriz_resultante \n");
        exit(1);
    }
}

void inicializa_matriz(int dimensao) {
    for (int linha = 0; linha < dimensao; linha++) {
        for (int coluna = 0; coluna < dimensao; coluna++) {

            // Inicializa as matrizes de entrada com valores randômicos de 1 a 4

            matriz_1[linha * dimensao + coluna] = 1 + rand() % 4;
            matriz_2[linha * dimensao + coluna] = 1 + rand() % 4;
            matriz_resultante[linha * dimensao + coluna] = 0;
        }
    }
}

void * executa_tarefa(void *arg){

    int thread_id = * (int *) arg;

    for (int linha = thread_id; linha < dimensao_matriz; linha += quantidade_threads){
        for (int coluna = 0; coluna < dimensao_matriz; coluna++) {
            for (int k = 0; k < dimensao_matriz; k++) {
                matriz_resultante[linha * dimensao_matriz + coluna] +=
                        matriz_1[linha * dimensao_matriz + k] * matriz_2[k * dimensao_matriz + coluna];
            }
        }
    }

    pthread_exit(NULL);
}

void aguarda_execucao_das_tarefas(pthread_t *thread_ids_sistema){
    for (int thread = 0; thread < quantidade_threads; thread++){
        int status_pthread_join = pthread_join(thread_ids_sistema[thread], NULL);

        if (status_pthread_join) {
            printf("ERRO; status code retornado no pthread_join() = %d\n", status_pthread_join);
            exit(-1);
        }
    }
}

void imprime_matrizes(int dimensao) {

    void imprime(float *matriz){
        for (int linha = 0; linha < dimensao; linha++) {
            for (int coluna = 0; coluna < dimensao; coluna++) {
                printf("%.0f ", matriz[linha * dimensao + coluna]);
            }
            puts("");
        }
    }

    puts("Primeira matriz de entrada: ");
    imprime(matriz_1);

    puts("Segunda matriz de entrada: ");
    imprime(matriz_2);

    puts("Matrz resultante: ");
    imprime(matriz_resultante);
}


void libera_memoria() {
    free(matriz_1);
    free(matriz_2);
    free(matriz_resultante);
}
