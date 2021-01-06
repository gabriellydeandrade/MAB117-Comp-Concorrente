#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include "main.h"

#define ANSI_COR_FALHA     "\x1b[31m"
#define ANSI_COR_SUCESSO   "\x1b[32m"
#define ANSI_COR_PADRAO   "\x1b[0m"


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

    if (argc > 1 && !strncmp(argv[argc-1], "--teste", 7)){
        // Caso o último parâmetro seja "--teste" na função main, ele irá executar o arquivo de testes
        teste();
    }
    else {
        // Executa esse bloco para avaliar desempenho com vetores de valores randômicos

        if (argc < 3) {
            printf("Informe o tamanho do vetor e o número de threads do programa <%s> por argumento \n", argv[0]);
            return 1;
        }

        long int tamanho_vetor = atoi(argv[1]);
        int qtd_threads = atoi(argv[2]);

        int *vetor;
        time_t t;

        // Inicializa a semente randômica que iremos usar na inicialização
        srand((unsigned) time(&t));

        // Inicializa com valores randomicos de 0 a 100 e exibe vetor antes
        vetor = malloc(tamanho_vetor * sizeof(int));
        for (int i = 0; i < tamanho_vetor; i++){
            vetor[i] = rand() % 100;
        }

        printf("Vetor antes \n");
        imprime_vetor(tamanho_vetor, vetor);

        // Chama a implementação do merge_sort concorrente
        merge_sort(tamanho_vetor, vetor, qtd_threads);

        printf("Vetor depois \n");
        imprime_vetor(tamanho_vetor, vetor);

        // Libera memória do vetor alocado dinamicamente
        free(vetor);
    }

    return 0;
}