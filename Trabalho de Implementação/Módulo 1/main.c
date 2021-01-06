#include <string.h>
#include "teste_merge_sort_concorrente.c"

#ifdef DEBUG
    #define debug if (1)
#else
    #define debug if (0)
#endif

int main(int argc, char *argv[]){
    /*
        Objetivo: Função principal que permite executar a ordenação de vetores ou chamar os testes de
                  validação dos cenários.

        Parâmetros:
            Flag "--teste": Poderá receber opcionalmente a flag para rodar os testes unitários.
                            Deve ser o primeiro parâmetro a ser informado.

        Retorno:
            Não há. Apenas há exibição no console.

    */

    if (argc > 1 && !strncmp(argv[1], "--teste", 7)){
        // Caso o primeiro parâmetro seja "--teste" na função main, ele irá executar o arquivo de testes
        teste();
    }
    else {
        // Executa esse bloco para avaliar desempenho com vetores de valores randômicos

        long int tamanho_vetor = 50;
        int qtd_threads = 4;
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

        // Chama a implementação do mergeSort concorrente
        mergeSort(tamanho_vetor, vetor, qtd_threads);

        printf("Vetor depois \n");
        imprime_vetor(tamanho_vetor, vetor);

        // Libera memória do vetor alocado dinamicamente
        free(vetor);
    }

    return 0;
}