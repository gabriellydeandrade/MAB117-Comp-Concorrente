#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

int qtd_threads;
int *vetor;
long int dimensao_vetor;

void inicializa_e_preenche_vetor();
void libera_vetor();

int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim;
    long int soma_sequencial = 0;
    long int soma_concorrente = 0;

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

void libera_vetor(){
    free(vetor);
}