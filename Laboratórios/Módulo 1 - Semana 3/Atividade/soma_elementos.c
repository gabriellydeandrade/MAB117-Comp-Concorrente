#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

int qtd_threads;
int *vetor;
long int dimensao_vetor;

void inicializa_e_preenche_vetor();
void *tarefa(void *arg);
void libera_vetor();

int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim;
    long int soma_sequencial = 0;
    long int soma_concorrente = 0;
    pthread_t *thread_id;
    long int soma_retornada_thread;

    if (argc < 3){
        fprintf(stderr,"Informe a dimensão do vetor e o número de threads do programa <%s> por argumento \n", argv[0]);
        return 1;
    }

    dimensao_vetor = atoi(argv[1]);
    qtd_threads = atoi(argv[2]);

    inicializa_e_preenche_vetor();

    // Soma sequencial dos elementos
    GET_TIME(tempo_inicio);

    for (long int i=0; i<dimensao_vetor; i++){
        soma_sequencial += vetor[i];
    }

    GET_TIME(tempo_fim);
    printf("Tempo sequencial: %lf\n", tempo_fim - tempo_inicio);

    // Soma concorrente dos elementos

    GET_TIME(tempo_inicio);

    thread_id = (pthread_t *) malloc(sizeof(pthread_t) * qtd_threads);
    if (!thread_id){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        return 1;
    }

    for (long int i=0; i<qtd_threads; i++){
        int status_pthread_create = pthread_create(thread_id + i, NULL, tarefa, (void *) i);

        if (status_pthread_create) {
            fprintf(stderr, "Status code retornado no pthread_create() = %d\n", status_pthread_create);
            return 2;
        }
    }

    for (long int i=0; i<qtd_threads; i++){
        int status_pthread_join = pthread_join(*(thread_id+i), (void **) &soma_retornada_thread);

        if (status_pthread_join) {
            fprintf(stderr, "Status code retornado no pthread_join() = %d\n", status_pthread_join);
            return 2;
        }

        soma_concorrente += soma_retornada_thread;
    }

    GET_TIME(tempo_fim);
    printf("Tempo concorrente: %lf\n", tempo_fim - tempo_inicio);


    printf("Soma sequencial: %ld\n", soma_sequencial);
    printf("Soma concorrente: %ld\n", soma_concorrente);
    libera_vetor();

    return 0;
}

void inicializa_e_preenche_vetor(){
    vetor = (int *) malloc(sizeof(int) * dimensao_vetor);

    if (!vetor){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        exit(1);
    }

    for (long int i=0; i<dimensao_vetor; i++){
        vetor[i] = i % 1000;
    }

}

void *tarefa(void *arg){
    long int id = (long int) arg;
    long int soma_local = 0;
    long int tamanho_bloco = dimensao_vetor / qtd_threads;
    long int inicio_bloco = id * tamanho_bloco;
    long int fim_bloco;

    if (id == qtd_threads - 1){
        fim_bloco = dimensao_vetor;
    }
    else {
        fim_bloco = inicio_bloco + tamanho_bloco;
    }

    for (int i=inicio_bloco; i<fim_bloco; i++){
        soma_local += vetor[i];
    }

    pthread_exit((void *) soma_local);

}

void libera_vetor(){
    free(vetor);
}