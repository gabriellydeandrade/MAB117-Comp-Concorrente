#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

int qtd_threads;
long long int qtd_elementos_aproximacao_pi;
double *vetor_soma;

void inicializa_e_preenche_vetor();
void *tarefa(void *arg);

int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim, delta_sequencial, delta_concorrente;
    double soma_sequencial = 0;
    double soma_concorrente = 0;
    pthread_t *thread_id;
    double *soma_retornada_thread;

    if (argc < 3){
        fprintf(stderr,"Informe o número de elementos da aproximação de pi e o número de threads do programa <%s> por argumento \n", argv[0]);
        return 1;
    }

    qtd_elementos_aproximacao_pi = atoll(argv[1]);
    qtd_threads = atoi(argv[2]);

    inicializa_e_preenche_vetor();

    // Aproximação de PI sequencial
    GET_TIME(tempo_inicio);

    for (long long int i=qtd_elementos_aproximacao_pi-1; i>=0; i--){
        soma_sequencial += vetor_soma[i];
    }

    printf("Aproximação de PI sequencial: %.15lf\n", 4 * soma_sequencial);
    printf("Valor de PI da biblioteca math.h: %.15lf\n", M_PI);
    printf("Erro absoluto calculado na aproximação sequencial: %.15lf\n", (4 * soma_sequencial) - M_PI);

    GET_TIME(tempo_fim);
    delta_sequencial = tempo_fim - tempo_inicio;
    printf("Tempo sequencial: %lf\n\n", delta_sequencial);

    // Aproximação de PI concorrente

    GET_TIME(tempo_inicio);

    thread_id = (pthread_t *) malloc(sizeof(pthread_t) * qtd_threads);
    if (!thread_id){
        fprintf(stderr, "Não foi possível alocar memória para o as threads usando o malloc \n");
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

        soma_concorrente += *soma_retornada_thread;
        free(soma_retornada_thread);
    }

    GET_TIME(tempo_fim);
    delta_concorrente = tempo_fim - tempo_inicio;

    printf("Aproximação de PI concorrente: %.15lf\n", 4 * soma_concorrente);
    printf("Valor de PI da biblioteca math.h: %.15lf\n", M_PI);
    printf("Erro absoluto calculado na aproximação concorrente: %.15lf\n", (4 * soma_concorrente) - M_PI);
    printf("Tempo concorrente: %lf\n\n", delta_concorrente);

    printf("Aceleração obtida para N=%lld: %.15lf\n", qtd_elementos_aproximacao_pi, delta_sequencial/delta_concorrente);

    return 0;
}

void inicializa_e_preenche_vetor(){
    vetor_soma = (double *) malloc(sizeof(double) * qtd_elementos_aproximacao_pi);

    if (!vetor_soma){
        fprintf(stderr, "Não foi possível alocar memória para o vetor_soma usando o malloc \n");
        exit(1);
    }

    double denominador = 1;
    for (long long int i=0; i<qtd_elementos_aproximacao_pi; i++){
        vetor_soma[i] = pow(-1, i) * (1/denominador);

        denominador += 2;
    }

}

void *tarefa(void *arg){
    long int id = (long int) arg;
    double *soma_local;

    soma_local = (double *) malloc(sizeof(double));
    if (!soma_local){
        fprintf(stderr, "Não foi possível alocar memória para a soma local usando o malloc \n");
        exit(1);
    }

    for (long long int i=id; i<qtd_elementos_aproximacao_pi; i+=qtd_threads){
        *soma_local += vetor_soma[i];
    }

    pthread_exit((void *) soma_local);

}