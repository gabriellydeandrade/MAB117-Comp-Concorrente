/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Trabalho de Implementação */
/* Merge Sort Concorrente */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int TAM_VETOR = 30;
int QTD_THREADS = 10;
int *vetor;
typedef struct {
    int inicio;
    int fim;
} threadArgs;

void mergeSort(int inicio, int fim);
void merge(int inicio, int meio, int fim);
void *tarefa(void *arg);
void imprime_vetor();


int main(){
    pthread_t *thread_id;
    threadArgs *arg;
    threadArgs *vetorThreadArgs[QTD_THREADS];
    int qtd_extra_total = TAM_VETOR % QTD_THREADS;
    int qtd_extra = 0;
    double tempo_inicial, tempo_final, tempo_delta;
    time_t t;

    // Inicializa a semente randômica que iremos usar na inicialização
    srand((unsigned) time(&t));

    // Inicializa com valores randomicos de 0 a 100 e exibe vetor antes
    vetor = malloc(TAM_VETOR * sizeof(int));
    for (int i = 0; i < TAM_VETOR; i++){
        vetor[i] = rand() % 100;
    }

    printf("Vetor antes \n");
    imprime_vetor();

    // Alocação das threads no trecho concorrente

    GET_TIME(tempo_inicial);

    thread_id = (pthread_t *) malloc(sizeof(pthread_t) * QTD_THREADS);
    if (!thread_id){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        return 1;
    }

    for (long int i=0; i<QTD_THREADS; i++){
        arg = malloc(sizeof(threadArgs));

        if (!arg) {
            printf("ERRO; não foi possível alocar memória\n");
            return 1;
        }

        arg->inicio = i * (TAM_VETOR/QTD_THREADS);
        arg->fim = (i + 1) * (TAM_VETOR/QTD_THREADS);

        arg->inicio += qtd_extra;
        arg->fim += qtd_extra;

        if (qtd_extra_total != 0){
            arg-> fim += 1;
            qtd_extra_total--;
            qtd_extra += 1;
        }

        vetorThreadArgs[i] = arg;

        int status_pthread_create = pthread_create(thread_id + i, NULL, tarefa, (void *) vetorThreadArgs[i]);
        if (status_pthread_create) {
            printf("ERRO; status code retornado no pthread_create() = %d\n", status_pthread_create);
            return 2;
        }
    }

    // Aguardando execução das threads

    for (long int i=0; i<QTD_THREADS; i++){
        int status_pthread_join = pthread_join(*(thread_id+i), NULL);
        if (status_pthread_join) {
            fprintf(stderr, "Status code retornado no pthread_join() = %d\n", status_pthread_join);
            return 2;
        }
    }

    // Junta cada parte ordenada pelas threads (trecho sequencial)

    int j = 0;
    if (QTD_THREADS > 1){
        int inicio, meio, fim;
        int id = 0;

        // A quantidade de junções deve ser igual a qtd de partes - 1
        for (int i=0; i<QTD_THREADS-1; i++){
            if (id+1 < QTD_THREADS){
                inicio = vetorThreadArgs[id]->inicio;
                meio = vetorThreadArgs[id]->fim;
                fim = vetorThreadArgs[id+1]->fim;
                id += 2;
            }
            else {
                inicio = 0;
                meio = vetorThreadArgs[j+1]->fim;

                if (j+3 >= QTD_THREADS)
                    fim = vetorThreadArgs[j+2]->fim;
                else
                    fim = vetorThreadArgs[j+3]->fim;
                j+=2;
            }
            merge(inicio, meio, fim);
        }
    }

    GET_TIME(tempo_final);
    tempo_delta = tempo_final - tempo_inicial;
    printf("Tempo total concorrente: %lf\n", tempo_delta);

    GET_TIME(tempo_inicial);

    printf("Vetor depois \n");
    imprime_vetor();
}

void mergeSort(int inicio, int fim){
    if (fim - inicio > 1){
        int meio = (inicio+fim)/2;

        mergeSort(inicio, meio);
        mergeSort(meio, fim);

        merge(inicio, meio, fim);
    }
}

void merge(int inicio, int meio, int fim){
    int qtd_elementos_esquerdo = meio - inicio;
    int qtd_elementos_direito = fim - meio;

    int esquerdo[qtd_elementos_esquerdo], direito[qtd_elementos_direito];
    int topo_esquerdo = 0, topo_direito = 0;

    for (int i=0; i<qtd_elementos_esquerdo; i++){
        esquerdo[i] = vetor[inicio + i];
    }

    for (int i=0; i<qtd_elementos_direito; i++){
        direito[i] = vetor[meio + i];
    }

    for (int i=inicio; i<fim; i++){
        if (topo_esquerdo >= qtd_elementos_esquerdo){
            vetor[i] = direito[topo_direito];
            topo_direito++;
        }
        else if (topo_direito >= qtd_elementos_direito){
            vetor[i] = esquerdo[topo_esquerdo];
            topo_esquerdo++;
        }
        else if (esquerdo[topo_esquerdo] < direito[topo_direito]){
            vetor[i] = esquerdo[topo_esquerdo];
            topo_esquerdo++;
        }
        else {
            vetor[i] = direito[topo_direito];
            topo_direito++;
        }
    }

}

void *tarefa(void *arg){
    threadArgs *args = (threadArgs *) arg;

    mergeSort(args->inicio, args->fim);

    pthread_exit(NULL);
}

void imprime_vetor(){
    for (int i=0; i<TAM_VETOR; i++){
        printf("%d ", vetor[i]);
    }
    printf("\n");
}