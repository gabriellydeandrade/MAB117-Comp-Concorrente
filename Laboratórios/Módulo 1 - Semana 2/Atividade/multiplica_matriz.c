/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Laboratório: Implementar um programa concorrente de multiplicação de matrizes quadradas */

#include <stdio.h>
#include <stdlib.h>

void aloca_memoria(int dimensao);
void inicializa_matriz(int dimensao);
void imprime_resultado(int dimensao);
void libera_memoria();

float *matriz_1, *matriz_2, *matriz_resultante;


int main(int argc, char *argv[]) {

    /*
        PARÂMETROS
        Recebe a dimensão da matriz. Por convenção estamos assumindo que a matriz é quadrada.

        DESCRIÇÃO
        Calcula a multiplicação da matriz de forma concorrente.

        VALOR RETORNADO
        Matriz resultante de dimensão N recebida por parâmetro.

    */


    int dimensao_matriz;

    if (argc < 2) {
        printf("Informe a dimensão da matriz do programa <%s> por argumento \n", argv[0]);
        return 1;
    }

    dimensao_matriz = atoi(argv[1]);

    aloca_memoria(dimensao_matriz);

    inicializa_matriz(dimensao_matriz);

    // Trecho do processamento que poderá ser feito de forma concorrente

    for (int linha = 0; linha < dimensao_matriz; linha++) {
        for (int coluna = 0; coluna < dimensao_matriz; coluna++) {
            for (int i = 0; i < dimensao_matriz; i++) {
                matriz_resultante[linha * dimensao_matriz + coluna] +=
                        matriz_1[linha * dimensao_matriz + i] * matriz_2[i * dimensao_matriz + coluna];
            }
        }
    }

    imprime_resultado(dimensao_matriz);

    libera_memoria();

    return 0;
}

void aloca_memoria(int dimensao) {
    matriz_1 = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if (!matriz_1) {
        printf("ERRO; não foi possível alocar memória para a matriz_1 \n");
        exit(1);
    }
    matriz_2 = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if (!matriz_2) {
        printf("ERRO; não foi possível alocar memória para a matriz_2 \n");
        exit(1);
    }

    matriz_resultante = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if (!matriz_resultante) {
        printf("ERRO; não foi possível alocar memória para a matriz_resultante \n");
        exit(1);
    }
}

void inicializa_matriz(int dimensao) {
    for (int linha = 0; linha < dimensao; linha++) {
        for (int coluna = 0; coluna < dimensao; coluna++) {
            matriz_1[linha * dimensao + coluna] = 1;
            matriz_2[linha * dimensao + coluna] = 1;
            matriz_resultante[linha * dimensao + coluna] = 0;
        }
    }
}

void imprime_resultado(int dimensao) {
    puts("Matriz de entrada: ");
    for (int linha = 0; linha < dimensao; linha++) {
        for (int coluna = 0; coluna < dimensao; coluna++) {
            printf("%.1f ", matriz_1[linha * dimensao + coluna]);
        }
        puts("");
    }
    puts("Matriz de entrada: ");
    for (int linha = 0; linha < dimensao; linha++) {
        for (int coluna = 0; coluna < dimensao; coluna++) {
            printf("%.1f ", matriz_2[linha * dimensao + coluna]);
        }
        puts("");
    }

    puts("Matriz de saída: ");
    for (int linha = 0; linha < dimensao; linha++) {
        for (int coluna = 0; coluna < dimensao; coluna++) {
            printf("%.1f ", matriz_resultante[linha * dimensao + coluna]);
        }
        puts("");
    }
}


void libera_memoria() {
    free(matriz_1);
    free(matriz_2);
    free(matriz_resultante);
}
