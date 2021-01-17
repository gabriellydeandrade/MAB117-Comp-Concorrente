/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Laboratório: 4 */
/* Laboratório: Utilziando sincronização de variáveis condicionais */

/***** Condicao logica da aplicacao: Implemente um programa com 4 threads. A thread 1 imprime a frase “tudo bem?”
A thread 2 imprime a frase “bom dia!” A thread 3 imprime a frase “até mais!” A
thread 4 imprime a frase “boa tarde!”. ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NTHREADS  4

int indice_saudacao = 0;
char saudacao[4][11] = {"tudo bem?", "bom dia!", "até mais!", "boa tarde!"};
pthread_mutex_t mutex;
pthread_cond_t condicao;
int debug = 0;

void *saudacao1(void *args) {
    int * thread_id = (int *) args;

    if (debug) printf("T%d: começou\n", *thread_id + 1);

    printf("T%d: %s \n", *thread_id + 1, saudacao[*thread_id]);

    pthread_mutex_lock(&mutex);
    indice_saudacao++;
    if (indice_saudacao == 2){
        if (debug) printf("T%d: vai sinalizar a condicao \n", *thread_id + 1);
        pthread_cond_broadcast(&condicao);
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void *saudacao2(void *args) {
    int * thread_id = (int *) args;

    if (debug) printf("T%d: começou \n", *thread_id + 1);

    pthread_mutex_lock(&mutex);
    if (indice_saudacao < 2){
        if (debug) printf("T%d: vai se bloquear \n", *thread_id + 1);
        pthread_cond_wait(&condicao, &mutex);
        if (debug) printf("T%d: recebeu sinal e mutex realocado \n", *thread_id + 1);
    }
    printf("T%d %s \n", *thread_id + 1, saudacao[*thread_id]);
    indice_saudacao++;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc > 1){
        if (!strncmp(argv[1], "--debug", 7)) debug = 1;
    }

    pthread_t thread_ids_sistema[NTHREADS];
    int thread_ids_local[NTHREADS] = {0, 1, 2, 3};

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condicao, NULL);

    int status_pthread_create;
    void *tarefa;

    // Cria as threads
    for (int i=0; i<NTHREADS; i++){
        if (i < 2) tarefa = saudacao1; else tarefa = saudacao2;
        status_pthread_create = pthread_create(&thread_ids_sistema[i], NULL, tarefa, &thread_ids_local[i]);

        if (status_pthread_create) {
            fprintf(stderr, "Status code retornado no pthread_create() = %d\n", status_pthread_create);
            return 1;
        }
    }

    // Aguarda execução das threads
    for (int i=0; i<NTHREADS; i++) {
        int status_pthread_join = pthread_join(thread_ids_sistema[i], NULL);
        if (status_pthread_join) {
            fprintf(stderr, "status code retornado no pthread_join() = %d\n", status_pthread_join);
            return 1;
        }
    }
    if (debug) printf("\nFIM\n");

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condicao);
    return 0;
}
