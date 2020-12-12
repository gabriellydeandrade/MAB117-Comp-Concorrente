/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Laboratório: Implementar um programa concorrente de multiplicação de matrizes quadradas */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void aloca_memoria(int dimensao);
void inicializa_matriz(int dimensao);
void imprime_resultado(int dimensao);
void libera_memoria();

float *matriz, *vetor, *resultado;
int dimensao_matriz, quantidade_threads;

void * tarefa(void *arg){

    int thread_id = * (int *) arg;
    printf("Thread %d \n", thread_id);

    for (int i = thread_id; i < dimensao_matriz; i += quantidade_threads){
        for (int coluna = 0; coluna < dimensao_matriz; coluna++){
            resultado[i] += matriz[i * dimensao_matriz + coluna] * vetor[coluna];
        }
    }

    pthread_exit(NULL);
}


int main(int argc, char* argv[]){

    if (argc < 3) {
        printf("Informe a dimensão da matriz e o número de threads do programa <%s> por argumento \n", argv[0]);
        return 1;
    }

    dimensao_matriz = atoi(argv[1]);
    quantidade_threads = atoi(argv[2]);

    pthread_t thread_ids_sistema[quantidade_threads];
    int thread_ids_local[quantidade_threads];

    if (quantidade_threads > dimensao_matriz){
        quantidade_threads = dimensao_matriz;
    }

    aloca_memoria(dimensao_matriz);

    inicializa_matriz(dimensao_matriz);

    // multiplicação da matriz pelo vetor (concorrência, exploração CPU Bound)

    for (int thread = 0; thread < quantidade_threads; thread++){

        thread_ids_local[thread] = thread;
        int status_pthread_create = pthread_create(&thread_ids_sistema[thread], NULL, tarefa, (void *) &thread_ids_local[thread]);

        if (status_pthread_create) {
            printf("ERRO; status code retornado no pthread_create() = %d\n", status_pthread_create);
            return 2;
        }
    }

    // Espera as threads terminarem

    for (int thread = 0; thread < quantidade_threads; thread++){
        thread_ids_local[thread] = thread;
        int status_pthread_join = pthread_join(thread_ids_sistema[thread], NULL);

        if (status_pthread_join) {
            printf("ERRO; status code retornado no pthread_join() = %d\n", status_pthread_join);
            return 2;
        }
    }

    imprime_resultado(dimensao_matriz);

    libera_memoria();

    return 0;
}

void aloca_memoria(int dimensao){
    matriz = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if (!matriz){
        printf("ERRO; não foi possível alocar memória para a matriz \n");
        exit(1);
    }
    vetor = (float *) malloc(sizeof(float) * dimensao);
    if (!vetor){
        printf("ERRO; não foi possível alocar memória para o vetor \n");
        exit(1);
    }

    resultado = (float *) malloc(sizeof(float) * dimensao);
    if (!resultado){
        printf("ERRO; não foi possível alocar memória para o resultado \n");
        exit(1);
    }
}

void inicializa_matriz(int dimensao){
    for (int linha = 0; linha < dimensao; linha++){
        for (int coluna = 0; coluna < dimensao; coluna++){
            matriz[linha * dimensao + coluna] = 1;
        }
        vetor[linha] = 1;
        resultado[linha] = 0;
    }
}

void imprime_resultado(int dimensao){
    puts("Matriz de entrada: ");
    for (int linha = 0; linha < dimensao; linha++){
        for (int coluna = 0; coluna < dimensao; coluna++){
            printf("%.1f ", matriz[linha * dimensao + coluna]);
        }
        puts("");
    }
    puts("Vetor de entrada: ");
    for (int i=0; i < dimensao; i++){
        printf("%.1f ", vetor[i]);
    }
    puts("");

    puts("Vetor de saída: ");
    for (int i=0; i < dimensao; i++){
        printf("%.1f ", resultado[i]);
    }
    puts("");
}


void libera_memoria(){
    free(matriz);
    free(vetor);
    free(resultado);
}
