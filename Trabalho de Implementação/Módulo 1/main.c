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
int VALIDAR = 0;
int RANDOMICO = 0;
int NTHREADS = 1;
int LER_ARQUIVO = 0;
char *NOME_ARQUIVO;
long long int TAM_VETOR;
int *VETOR;
int OUTPUT = 0;
int SUMARIO = 0;

void ler_parametros(int qtd_params, char *params[]){
    for(int i=1; i<qtd_params; i++){
        if(!strncmp(params[i], "--teste", 7)){ teste(); exit(0);}
        if(!strncmp(params[i], "--debug", 7)) DEBUG=1;
        if(!strncmp(params[i], "--validar", 9)) VALIDAR=1;
        if(!strncmp(params[i], "--tamanho_vetor", 15)) {
            if (i+1 >= qtd_params) puts("Informe a quantidade de elementos do vetor após a flag --tamanho_vetor");
            else TAM_VETOR=atoi(params[++i]);;
        }
        if(!strncmp(params[i], "--threads", 9)) {
            if (i+1 >= qtd_params) puts("Informe a quantidade de threads após a flag --threads");
            else NTHREADS=atoi(params[++i]);
        }
        if(!strncmp(params[i], "--arquivo", 9)){
            LER_ARQUIVO=1;
            if (i+1 >= qtd_params) puts("Informe o nome do arquivo após a flag --arquivo");
            else NOME_ARQUIVO=params[++i];
        }
        if(!strncmp(params[i], "--randomico", 11)) RANDOMICO=1;
        if(!strncmp(params[i], "--output", 8)) OUTPUT=1;
        if(!strncmp(params[i], "--sumario", 9)) SUMARIO=1;
    }

    if (!LER_ARQUIVO && DEBUG) printf("Ordenando vetor com %lld elementos com %d threads\n", TAM_VETOR, NTHREADS);

    // Validações
    if (!LER_ARQUIVO && TAM_VETOR == 0){
        printf("A quantidade de elementos do vetor deve ser maior do que zero. Informe corretamente com a flag --tamanho_vetor\n");
        exit(1);
    }
    if (LER_ARQUIVO && RANDOMICO) {
        fprintf(stderr, "Não é possível utilizar a flag --arquivo com a flag --randomico\n");
        exit(1);
    }
    if (!LER_ARQUIVO && !RANDOMICO) {
        RANDOMICO = 1;
    }
    if(NTHREADS < 1) {
        fprintf(stderr, "O número de threads deve ser maior que zero\n");
        exit(1);
    }

};

void ler_vetor_em_arquivo(char *nome_arquivo){
    if (DEBUG) printf("Lendo o arquivo: %s\n", nome_arquivo);
    
    FILE *arquivo;
    arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL){
        printf("ERRO: Não foi possível abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    fscanf(arquivo, "%lld", &TAM_VETOR);
    
    VETOR = malloc(TAM_VETOR * sizeof(int)); 
    if(!VETOR){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        exit(1);
    }

    if (DEBUG) printf("Lendo %lld elemento(s) do arquivo com %d thread(s)\n", TAM_VETOR, NTHREADS);
    
    for (int i=0; i<TAM_VETOR; i++)
        fscanf(arquivo, "%d", &VETOR[i]);
}

void validar_vetor_ordenado(){
    for(int i=0; i<TAM_VETOR-1; i++){
        int atual = VETOR[i];
        int proximo = VETOR[i+1];
        if(atual > proximo){
            fprintf(stderr, "Erro ao ordenar vetor %d > %d\n", atual, proximo);
            exit(1);
        }
    }
    printf("Vetor ordenado de forma correta!\n");
}

void inicializa_randomico(){
    time_t t;

    // Inicializa a semente randômica que iremos usar na inicialização
    srand((unsigned) time(&t));

    VETOR = malloc(TAM_VETOR * sizeof(int));
    if(!VETOR){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        exit(1);
    }

    for (int i = 0; i < TAM_VETOR; i++)
        VETOR[i] = rand() % 100;
};


int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim, tempo_delta;

    /*
        Objetivo: Função principal que permite executar a ordenação de vetores ou chamar os testes de
                  validação dos cenários.

        Parâmetros:
            Flag "--teste": poderá receber opcionalmente a flag para rodar os testes unitários.
            Flag "--arquivo": permite enviar um arquivo para o vetor ser ordenado.
                              Na primeira linha encontra-se a quantidade de elementos e a quantidade de threads separados por espaço.
                              Na segunda, encontra-se os elementos do vetor separados por espaço.
            Flag "--output": exibe o resultado da ordenação no console.
            Flag "--debug": exibe resultados de log como vetor antes e depois e o tempo da ordenação.
            Flag "--sumario": exibe o tempo que levou para ordenar.
            Flag "--validar": realiza um teste de caixa preta da ordenação e exibe uma mensagem se foi ordenado correto ou não.
            Flag "--tamanho_vetor": recebe o tamanho do vetor a ser ordenado.
            Flag "--threads": recebe a quantidade de threads. Caso não seja definida o padrão é 1 thread (simulando comportamento sequencial).

        Retorno:
            Não há. Apenas há exibição no console caso sejam passadas as flags corretas.
    */

    ler_parametros(argc, argv);
    
    if(LER_ARQUIVO) ler_vetor_em_arquivo(NOME_ARQUIVO);

    if(RANDOMICO) inicializa_randomico();

    if (DEBUG) printf("Vetor antes \n");
    if (DEBUG) imprime_vetor(TAM_VETOR, VETOR);

    // Chama a implementação do merge_sort concorrente e adiciona tomada de tempo
    GET_TIME(tempo_inicio);

    merge_sort_concorrente(TAM_VETOR, VETOR, NTHREADS);

    GET_TIME(tempo_fim);
    tempo_delta = tempo_fim - tempo_inicio;
    if (DEBUG || SUMARIO) printf("Tempo para ordenação: %lf\n", tempo_delta);

    if (DEBUG) printf("Vetor depois \n");
    if (OUTPUT || DEBUG) imprime_vetor(TAM_VETOR, VETOR);

    if (VALIDAR) validar_vetor_ordenado();

    // Libera memória do vetor alocado dinamicamente
    free(VETOR);

    return 0;
}
