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
int READ_FILE = 0;
char *FILENAME;
int TAM_VETOR;
int *VETOR;

// limitar qtd de threads
// passar por arquivo
// 

int ler_parametros(int qtd_params, char *params[]){
    for(int i=1; i<qtd_params; i++){
        if(!strncmp(params[i], "--teste", 7)){ teste(); return 0;};
        if(!strncmp(params[i], "--debug", 7)) DEBUG=1;
        if(!strncmp(params[i], "--validar", 9)) VALIDA=1;
        if(!strncmp(params[i], "--threads", 9)) NTHREADS=atoi(params[++i]);
        if(!strncmp(params[i], "--file", 6)){ READ_FILE=1; FILENAME=params[++i]; };
        if(!strncmp(params[i], "--randomico", 11)) RANDOMICO=1;
    };
    return 0;
};

void ler_vetor_em_arquivo(char *filename){
    if (DEBUG) printf("Lendo o arquivo: %s\n", filename);
    
    FILE *myFile;
    myFile = fopen(filename, "r");

    fscanf(myFile, "%d", &TAM_VETOR);
    
    VETOR = malloc(TAM_VETOR * sizeof(int)); 
    if(!VETOR){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        exit(1);
    }

    if (DEBUG) printf("Lendo %d de elementos do arquivo\n", TAM_VETOR);
    
    for (int i=0; i<TAM_VETOR; i++)
        fscanf(myFile, "%d", &VETOR[i]);

}

int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim, tempo_delta;

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
    if(READ_FILE){
        ler_vetor_em_arquivo(FILENAME);
    }

    if(RANDOMICO){        
        time_t t;                

        // Inicializa a semente randômica que iremos usar na inicialização
        srand((unsigned) time(&t));

        for (int i = 0; i < TAM_VETOR; i++)
            VETOR[i] = rand() % 100;
    }

    if (DEBUG) printf("Vetor antes \n");
    if (DEBUG) imprime_vetor(TAM_VETOR, VETOR);

    // Chama a implementação do merge_sort concorrente e adiciona tomada de tempo
    GET_TIME(tempo_inicio);

    merge_sort(TAM_VETOR, VETOR, NTHREADS);

    GET_TIME(tempo_fim);
    tempo_delta = tempo_fim - tempo_inicio;
    printf("Tempo sequencial: %lf\n\n", tempo_delta);

    if (DEBUG) printf("Vetor depois \n");
    if (DEBUG) imprime_vetor(TAM_VETOR, VETOR);

    // Libera memória do vetor alocado dinamicamente
    free(VETOR);

    return 0;
}
