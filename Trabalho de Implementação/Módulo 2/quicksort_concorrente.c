/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Trabalho de Implementação */
/* Quicksort Concorrente */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *vetor;
    int inicio;
    int fim;
} threadArgs;

int *quicksort_concorrente(long long int tamanho_vetor, int *vetor, int qtd_threads);
void quicksort(int *vetor, int inicio, int fim);
int particao(int *vetor, int inicio, int fim);
void *tarefa(void *arg);
void imprime_vetor(long long int tamanho_vetor, int *vetor);


int *quicksort_concorrente(long long int tamanho_vetor, int *vetor, int qtd_threads){
    pthread_t *thread_id;
    threadArgs *arg;
    threadArgs *vetorThreadArgs[qtd_threads];
    int qtd_extra_total = tamanho_vetor % qtd_threads;
    int qtd_extra = 0;

    // Alocação das threads no trecho concorrente

    thread_id = (pthread_t *) malloc(sizeof(pthread_t) * qtd_threads);
    if (!thread_id){
        fprintf(stderr, "Não foi possível alocar memória para o vetor de identificação das threads usando o malloc \n");
        exit(1);
    }

    for (long int i=0; i<qtd_threads; i++){
        arg = malloc(sizeof(threadArgs));

        if (!arg) {
            printf("ERRO; não foi possível alocar memória para o argumento da thread usando malloc\n");
            exit(1);
        }

        // TODO modificar esse trecho para permitir utilizar bolsa de tarefas (buffer)

        arg->vetor = vetor;
        arg->inicio = i * (tamanho_vetor/qtd_threads);
        arg->fim = (i + 1) * (tamanho_vetor/qtd_threads);

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
            exit(1);
        }
    }

    // Aguardando execução das threads

    for (long int i=0; i<qtd_threads; i++){
        int status_pthread_join = pthread_join(*(thread_id+i), NULL);
        if (status_pthread_join) {
            fprintf(stderr, "Status code retornado no pthread_join() = %d\n", status_pthread_join);
            exit(1);
        }
    }

//    TODO ver maneira de juntar cada parte ordenada pelas threads (trecho sequencial). Está errado para threads > 1

    return vetor;
}

void quicksort(int *vetor, int inicio, int fim){
    if (fim - inicio > 1){
        int pos_pivo = particao(vetor, inicio, fim-1);

        quicksort(vetor, inicio, pos_pivo);
        quicksort(vetor, pos_pivo+1, fim);

    }
}

int particao(int *vetor, int inicio, int fim){
    int pivo = vetor[fim];
    int pos_menores = inicio, pos_maiores, temp;

    for (pos_maiores = inicio; pos_maiores < fim; pos_maiores++){
        if (vetor[pos_maiores] <= pivo){
            temp = vetor[pos_menores];
            vetor[pos_menores] = vetor[pos_maiores];
            vetor[pos_maiores] = temp;

            pos_menores++;
        }
    }

    temp = vetor[pos_menores];
    vetor[pos_menores] = vetor[fim];
    vetor[fim] = temp;

    return pos_menores;
}

void *tarefa(void *arg){
    threadArgs *args = (threadArgs *) arg;

    quicksort(args->vetor, args->inicio, args->fim);

    pthread_exit(NULL);
}

void imprime_vetor(long long int tamanho_vetor, int *v){
    for (int i=0; i<tamanho_vetor; i++){
        printf("%d ", v[i]);
    }
    printf("\n");
}
