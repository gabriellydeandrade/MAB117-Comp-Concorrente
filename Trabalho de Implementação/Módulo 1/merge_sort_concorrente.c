/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Trabalho de Implementação */
/* Merge Sort Concorrente */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int *vetor;
    int inicio;
    int fim;
} threadArgs;

int *merge_sort_concorrente(long long int tamanho_vetor, int *vetor, int qtd_threads);
void merge_sort(int *vetor, int inicio, int fim);
void merge(int *vetor, int inicio, int meio, int fim);
void *tarefa(void *arg);
void imprime_vetor(long long int tamanho_vetor, int *vetor);


int *merge_sort_concorrente(long long int tamanho_vetor, int *vetor, int qtd_threads){
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

    // Junta cada parte ordenada pelas threads (trecho sequencial)

    int j = 0;
    int inicio, meio, fim;
    int id = 0;

    // A quantidade de junções deve ser igual a qtd de partes - 1
    for (int i=0; i<qtd_threads-1; i++){
        if (id+1 < qtd_threads){
            inicio = vetorThreadArgs[id]->inicio;
            meio = vetorThreadArgs[id]->fim;
            fim = vetorThreadArgs[id+1]->fim;
            id += 2;
        }
        else {
            inicio = 0;
            meio = vetorThreadArgs[j+1]->fim;

            if (j+3 >= qtd_threads)
                fim = vetorThreadArgs[j+2]->fim;
            else
                fim = vetorThreadArgs[j+3]->fim;
            j+=2;
        }
        merge(vetor, inicio, meio, fim);
    }

    return vetor;
}

void merge_sort(int *vetor, int inicio, int fim){
    if (fim - inicio > 1){
        int meio = (inicio+fim)/2;

        merge_sort(vetor, inicio, meio);
        merge_sort(vetor, meio, fim);

        merge(vetor, inicio, meio, fim);
    }
}

void merge(int *vetor, int inicio, int meio, int fim){
    int qtd_elementos_esquerdo = meio - inicio;
    int qtd_elementos_direito = fim - meio;
    int topo_esquerdo = 0, topo_direito = 0;
    int *esquerdo = malloc(qtd_elementos_esquerdo * sizeof(int));
    if (!esquerdo){
        fprintf(stderr, "Não foi possível alocar memória para o vetor da metade esquerda usando o malloc \n");
        exit(1);
    }
    int *direito = malloc(qtd_elementos_direito * sizeof(int));
    if (!direito){
        fprintf(stderr, "Não foi possível alocar memória para o vetor da metade direita usando o malloc \n");
        exit(1);
    }

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

    free(esquerdo);
    free(direito);
}

void *tarefa(void *arg){
    threadArgs *args = (threadArgs *) arg;

    merge_sort(args->vetor, args->inicio, args->fim);

    pthread_exit(NULL);
}

void imprime_vetor(long long int tamanho_vetor, int *v){
    for (int i=0; i<tamanho_vetor; i++){
        printf("%d ", v[i]);
    }
    printf("\n");
}
