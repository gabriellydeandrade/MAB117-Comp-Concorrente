/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Laboratório: Implementar um programa concorrente de multiplicação de matrizes quadradas */

#include <stdio.h>
#include <stdlib.h>

// alocação dinâmica
float *matriz, *vetor, *resultado;

// argc quantidade de argumentos
// argv lista de argumentos
int main(int argc, char* argv[]){
    int dimensao_matriz;

    // leitura e avaliação dos parâmetros de entrada

    // o primeiro argumento do argv é o nome do programa
    if (argc < 2) {
        printf("Informe a dimensão da matriz do programa <%s> por argumento \n", argv[0]);
        return 1;
    }

    //converte string para inteiro
    dimensao_matriz = atoi(argv[1]);

    // alocação de memória de forma dinâmica para as estruturas de dados (matriz)

    // TODO converter essas alocações para uma função

    matriz = (float *) malloc(sizeof(float) * dimensao_matriz * dimensao_matriz);
    if (!matriz){
        printf("ERRO; não foi possível alocar memória para a matriz \n");
        exit(1);
    }

    vetor = (float *) malloc(sizeof(float) * dimensao_matriz);
    if (!vetor){
        printf("ERRO; não foi possível alocar memória para o vetor \n");
        exit(1);
    }

    resultado = (float *) malloc(sizeof(float) * dimensao_matriz);
    if (!resultado){
        printf("ERRO; não foi possível alocar memória para o resultado \n");
        exit(1);
    }

    // inicialização das estruturas de dados (matriz) com valor default

    for (int linha = 0; linha < dimensao_matriz; linha++){
        for (int coluna = 0; coluna < dimensao_matriz; coluna++){
            // como a matriz é quadrada o salto é com relação a dimensão, se não fosse seria com relação à colunas
            matriz[linha * dimensao_matriz + coluna] = 1; //equivalente a matriz[linha][coluna]

        vetor[linha] = 1;
        resultado[linha] = 0;
        }
    }

    // multiplicação da matriz pelo vetor (concorrência, exploração CPU Bound)

    for (int linha = 0; linha<dimensao_matriz; linha++){
        for (int coluna = 0; coluna < dimensao_matriz; coluna++){
            resultado[linha] += matriz[linha * dimensao_matriz + coluna] * vetor[coluna];
        }
    }

    // exibição dos resultados
    puts("Matriz de entrada: ");
    for (int linha = 0; linha < dimensao_matriz; linha++){
        for (int coluna = 0; coluna < dimensao_matriz; coluna++){
            printf("%.1f ", matriz[linha * dimensao_matriz + coluna]);
        }
        puts("");
    }
    puts("Vetor de entrada: ");
    for (int i=0; i < dimensao_matriz; i++){
        printf("%.1f ", vetor[i]);
    }
    puts("");

    puts("Vetor de saída: ");
    for (int i=0; i < dimensao_matriz; i++){
        printf("%.1f ", resultado[i]);
    }
    puts("");

    // liberar memória
    free(matriz);
    free(vetor);
    free(resultado);

    return 0;
}



