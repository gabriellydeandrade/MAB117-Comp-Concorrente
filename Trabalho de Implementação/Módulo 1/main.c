#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include "merge.h"
#include "timer.h"

#define ANSI_COR_FALHA    "\x1b[31m"
#define ANSI_COR_SUCESSO  "\x1b[32m"
#define ANSI_COR_PADRAO   "\x1b[0m"

int DEBUG = 0;
int VALIDA = 0;
int RANDOMICO = 0;
int NTHREADS = 1;

// limitar qtd de threads
// passar por arquivo
// 

int ler_parametros(int qtd_params, char *params[]){
    for(int i=1; i<qtd_params; i++){
        printf("%s\n", params[i]);
        if(!strncmp(params[i], "--teste", 7)){ teste(); return 0;};
        if(!strncmp(params[i], "--debug", 7)){ DEBUG=1; };
        if(!strncmp(params[i], "--validar", 9)){ VALIDA=1; };
        if(!strncmp(params[i], "--threads", 9)){ NTHREADS=atoi(params[++i]); };  
    };
    return 0;
};

int main(int argc, char *argv[]){
    /*
        Objetivo: Função principal que permite executar a ordenação de vetores ou chamar os testes de
                  validação dos cenários.

        Parâmetros:
            Quantidade de elementos no vetor: <long int> primeiro parâmetro.
            Quantidade de threads: <int> segundo parâmetro.
            Flag "--teste": poderá receber opcionalmente a flag para rodar os testes unitários.
                            Deve ser o último parâmetro a ser informado.

        Retorno:
            Não há. Apenas há exibição no console.

    */

    ler_parametros(argc, argv);
    
    long int tamanho_vetor = 4000;
    int qtd_threads = NTHREADS;
    double tempo_inicio, tempo_fim, tempo_delta;
    int *vetor;
    time_t t;

    // Inicializa a semente randômica que iremos usar na inicialização
    srand((unsigned) time(&t));

    // Inicializa com valores randomicos de 0 a 100 e exibe vetor antes
    vetor = malloc(tamanho_vetor * sizeof(int));
    if (!vetor){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        exit(1);
    }

    for (int i = 0; i < tamanho_vetor; i++){
        vetor[i] = rand() % 100;
    }

    if (DEBUG) printf("Vetor antes \n");
    if (DEBUG) imprime_vetor(tamanho_vetor, vetor);

    // Chama a implementação do merge_sort concorrente e adiciona tomada de tempo
    GET_TIME(tempo_inicio);

    merge_sort(tamanho_vetor, vetor, qtd_threads);

    GET_TIME(tempo_fim);
    tempo_delta = tempo_fim - tempo_inicio;
    printf("Tempo sequencial: %lf\n\n", tempo_delta);

    if (DEBUG) printf("Vetor depois \n");
    if (DEBUG) imprime_vetor(tamanho_vetor, vetor);

    // Libera memória do vetor alocado dinamicamente
    free(vetor);

    return 0;
}
