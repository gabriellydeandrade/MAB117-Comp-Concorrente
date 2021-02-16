/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Trabalho de Implementação */
/* Quicksort Concorrente */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM_BUFFER 100

typedef struct {
    int *vetor;
    int inicio;
    int fim;
} threadArgs;

int *quicksort_concorrente(long long int tamanho_vetor, int *vetor, int qtd_threads);
void quicksort(int *vetor, int inicio, int fim);
int particao(int *vetor, int inicio, int fim);
void produtor_tarefa(int *vetor, int inicio, int fim);
void *consumidor_tarefa(void *arg);
void imprime_vetor(long long int tamanho_vetor, int *vetor);

threadArgs *buffer[TAM_BUFFER];
int contador_buffer=0, entrada=0, saida=0, contador_tarefas_enviadas=0, contador_tarefas_recebidas=0;

pthread_mutex_t mutex;
pthread_cond_t cond_cons, cond_prod;

void insere_buffer(threadArgs *item) {
    pthread_mutex_lock(&mutex);
    printf("P quer inserir, contador do buffer: %d\n", contador_buffer);
    while(contador_buffer == TAM_BUFFER) {
        printf("P bloqueou\n");
        pthread_cond_wait(&cond_prod, &mutex);
        printf("P desbloqueou\n");
    }
    buffer[entrada] = item;
    entrada = (entrada + 1) % TAM_BUFFER;
    contador_buffer++;
    printf("P inseriu\n");
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_cons);
}

threadArgs *retira_buffer(int id) {
    threadArgs *item;
    pthread_mutex_lock(&mutex);
    printf("C[%d] quer consumir tarefa %d/%d \n", id, contador_tarefas_recebidas, contador_tarefas_enviadas);
    while(contador_buffer == 0) {
        printf("C[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_cons, &mutex);
        printf("C[%d] desbloqueou\n", id);
    }
    item = buffer[saida];
    saida = (saida + 1) % TAM_BUFFER;
    contador_buffer--;
    printf("C[%d] consumiu tarefa %d/%d \n", id, contador_tarefas_recebidas, contador_tarefas_enviadas);
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_prod);
    return item;
}


int *quicksort_concorrente(long long int tamanho_vetor, int *vetor, int qtd_threads){
    pthread_t *thread_id;
    int id[qtd_threads];

    // Alocação das threads no trecho concorrente

    thread_id = (pthread_t *) malloc(sizeof(pthread_t) * qtd_threads);
    if (!thread_id){
        fprintf(stderr, "Não foi possível alocar memória para o vetor de identificação das threads usando o malloc \n");
        exit(1);
    }

    quicksort(vetor, 0, tamanho_vetor); // Inicio da bolsa de tarefas com quick sort

    for (long int i = 0; i < qtd_threads; i++){
        id[i] = i+1;

        int status_pthread_create = pthread_create(thread_id + i, NULL, consumidor_tarefa, &id[i]);
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

    return vetor;
}

void quicksort(int *vetor, int inicio, int fim){
    if (fim - inicio > 1){
        int pos_pivo = particao(vetor, inicio, fim-1);

        produtor_tarefa(vetor, inicio, pos_pivo);
        produtor_tarefa(vetor, pos_pivo+1, fim);

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

void produtor_tarefa(int *vetor, int inicio, int fim){
    threadArgs *arg = malloc(sizeof(threadArgs));

    if (!arg) {
        printf("ERRO; não foi possível alocar memória para o argumento da thread usando malloc\n");
        exit(1);
    }

    arg->inicio = inicio;
    arg->fim = fim;
    arg->vetor = vetor;

    pthread_mutex_lock(&mutex);
    contador_tarefas_enviadas++;
    pthread_mutex_unlock(&mutex);

    // FIXME quando o buffer está cheio ele leva o lock junto e a aplicação entra em deadlock (ocorre para vetores maiores que o buffer
    insere_buffer(arg);

}

void *consumidor_tarefa(void * arg) {
    int *id = (int *) arg;
    threadArgs *item;
    printf("Inicio da thread[%d]\n", *id);

    // FIXME esse loop está gerando algumas incongruencias
    while(contador_tarefas_recebidas < contador_tarefas_enviadas) {
        pthread_mutex_lock(&mutex);
        contador_tarefas_recebidas++;
        pthread_mutex_unlock(&mutex);

        item = retira_buffer(*id);
        quicksort(item->vetor, item->inicio, item->fim);

        printf("tarefas %d/%d \n", contador_tarefas_recebidas, contador_tarefas_enviadas);

    }

    pthread_exit(NULL);
}

void imprime_vetor(long long int tamanho_vetor, int *v){
    for (int i=0; i<tamanho_vetor; i++){
        printf("%d ", v[i]);
    }
    printf("\n");
}
