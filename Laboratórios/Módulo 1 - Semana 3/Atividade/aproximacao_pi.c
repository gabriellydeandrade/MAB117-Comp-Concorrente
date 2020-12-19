#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

int qtd_threads;
long long int qtd_elementos_aproximacao_pi;
double *vetor_soma;

void inicializa_e_preenche_vetor();

int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim;
    double soma_sequencial = 0;

    if (argc < 3){
        fprintf(stderr,"Informe o número de elementos da aproximação de pi e o número de threads do programa <%s> por argumento \n", argv[0]);
        return 1;
    }

    qtd_elementos_aproximacao_pi = atoll(argv[1]);
    qtd_threads = atoi(argv[2]);

    inicializa_e_preenche_vetor();

    // Aproximação de PI sequencial
    GET_TIME(tempo_inicio);

    for (long long int i=0; i<qtd_elementos_aproximacao_pi; i++){
        soma_sequencial += vetor_soma[i];
    }

    printf("Aproximação de PI sequencial: %.15lf\n", 4 * soma_sequencial);
    printf("Valor de PI da biblioteca math.h: %.15lf\n", M_PI);
    printf("Erro absoluto calculado na aproximação sequencial: %.15lf\n", (4 * soma_sequencial) - M_PI);

    GET_TIME(tempo_fim);
    printf("Tempo sequencial: %lf\n\n", tempo_fim - tempo_inicio);

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