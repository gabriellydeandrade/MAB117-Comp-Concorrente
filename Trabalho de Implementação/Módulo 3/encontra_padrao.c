/*
 * Fazer casos de teste
 * */


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"

typedef struct retorno_padrao_a{
    long long int pos_inicial, tamanho;
    int valor;
} retorno_padrao_a;

retorno_padrao_a valores_identicos;
long long int qtd_triplas=0, qtd_seq_1_a_5=0;

int *buffer, tamanho_buffer, tamanho_bloco, ultimo_bloco=-1, tamanho_ultimo_bloco=-1;
FILE *arquivo;

char *status_buffer;
// status=0: linha livre
// status=1: falta 1 thread para leitura
// status=2: faltam 2 threads para leitura
// status=3: faltam 3 threads para leitura

pthread_mutex_t *locks;
pthread_cond_t cond_escrita, cond_leitura, cond_barreira;

int qtd_threads_chegaram=0;

void barreira(int pos_leitura) {
    /*
     * Força as threads leitores esperarem as demais caso a mesma linha seja utilizada novamente pelo escritor.
     */

    pthread_mutex_lock(locks+pos_leitura);
    if (qtd_threads_chegaram == 2) {
        pthread_cond_broadcast(&cond_barreira);
        qtd_threads_chegaram=0;
    } else {
        qtd_threads_chegaram++;
        pthread_cond_wait(&cond_barreira, locks+pos_leitura);
    }
    pthread_mutex_unlock(locks+pos_leitura);
}


void *insere_buffer(void *args){
    int pos_escrita=0; long long int qtd_inteiros_lidos, qtd_inteiros_faltantes;
    fread(&qtd_inteiros_faltantes, sizeof(long long int), 1, arquivo);

    while(qtd_inteiros_faltantes > 0){

        // Entrada na escrita

        pthread_mutex_lock(locks+pos_escrita);

        while(status_buffer[pos_escrita] > 0){
            // Espera enquanto faltar threads para leitura
            pthread_cond_wait(&cond_escrita, locks+pos_escrita);
        }

        pthread_mutex_unlock(locks+pos_escrita);

        // Escrita

        if (qtd_inteiros_faltantes > tamanho_bloco){
            qtd_inteiros_lidos = fread(buffer+(pos_escrita*tamanho_bloco), sizeof(int), tamanho_bloco, arquivo);
        } else {
            qtd_inteiros_lidos = fread(buffer+(pos_escrita*tamanho_bloco), sizeof(int), qtd_inteiros_faltantes, arquivo);
            ultimo_bloco = pos_escrita;
            tamanho_ultimo_bloco = qtd_inteiros_lidos;

        }


        qtd_inteiros_faltantes = qtd_inteiros_faltantes-qtd_inteiros_lidos;

        // Saida da escrita

        pthread_mutex_lock(locks+pos_escrita);
        status_buffer[pos_escrita] = 3;
        pthread_mutex_unlock(locks+pos_escrita);

        pos_escrita = (pos_escrita + 1) % tamanho_buffer;

        pthread_cond_broadcast(&cond_leitura);
    }

    pthread_exit(NULL);
}

void *padrao_a(void *args){
    /*
     * Busca a maior sequência de valores idênticos.
     * Informando a posição, tamanho da sequência e valor da sequência
     */

    int pos_leitura=0, tamanho=tamanho_bloco;

    long long int pos_inicial=0, tamanho_seq=0, pos_atual=1;
    long long int pos_temp, tamanho_temp=0;
    int valor=0, valor_temp;

    while(1){

        // Entrada para leitura

        pthread_mutex_lock(locks+pos_leitura);

        while(status_buffer[pos_leitura] == 0){
            // Espera enquanto a linha estiver vazia
            pthread_cond_wait(&cond_leitura, locks+pos_leitura);
        }

        pthread_mutex_unlock(locks+pos_leitura);

        // Leitura

        if (pos_leitura == ultimo_bloco){
            tamanho = tamanho_ultimo_bloco;
        }


        for (int i=0; i<tamanho; i++, pos_atual++){
            int sequencia = buffer[pos_leitura*tamanho_bloco+i];

            if (tamanho_temp == 0){
                valor_temp = sequencia;
                pos_temp = pos_atual;
            }

            if (sequencia == valor_temp){
                tamanho_temp++;
            } else {
                if (tamanho_temp > tamanho_seq){
                    pos_inicial = pos_temp;
                    tamanho_seq = tamanho_temp;
                    valor = valor_temp;
                }

                tamanho_temp = 1;
                valor_temp = sequencia;
                pos_temp = pos_atual;
            }
        }

        // Saída da leitura

        pthread_mutex_lock(locks+pos_leitura);
        status_buffer[pos_leitura]--;

        if (status_buffer[pos_leitura] == 0) pthread_cond_signal(&cond_escrita);

        if (pos_leitura==ultimo_bloco){
            pthread_mutex_unlock(locks+pos_leitura);
            break;
        }
        pthread_mutex_unlock(locks+pos_leitura);

        pos_leitura = (pos_leitura + 1) % tamanho_buffer;
        if (pos_leitura == 0){
            barreira(pos_leitura);
        }
    }

    if (tamanho_temp > tamanho_seq){
        pos_inicial = pos_temp;
        tamanho_seq = tamanho_temp;
        valor = valor_temp;
    }

    valores_identicos.pos_inicial = pos_inicial;
    valores_identicos.tamanho = tamanho_seq;
    valores_identicos.valor = valor;

    pthread_exit(NULL);
}

void *padrao_b(void *args){
    /*
     * Busca a quantidade de triplas.
     */

    int pos_leitura=0, tamanho=tamanho_bloco;

    int valor, contador = 0;

    while(1){

        // Entrada para leitura

        pthread_mutex_lock(locks+pos_leitura);

        while(status_buffer[pos_leitura] == 0){ // Enquanto a linha estiver vazia
            pthread_cond_wait(&cond_leitura, locks+pos_leitura);
        }

        pthread_mutex_unlock(locks+pos_leitura);

        // Leitura

        if (pos_leitura == ultimo_bloco){
            tamanho = tamanho_ultimo_bloco;
        }


        for (int i=0; i<tamanho; i++){
            int sequencia = buffer[pos_leitura*tamanho_bloco+i];

            if (contador == 0){
                valor = sequencia;
            }

            if (sequencia == valor){
                contador++;
            } else {
                contador = 0;
            }

            if (contador == 3){
                qtd_triplas++;
                contador = 0;
            }
        }

        // Saída da leitura

        pthread_mutex_lock(locks+pos_leitura);
        status_buffer[pos_leitura]--;

        if (status_buffer[pos_leitura] == 0) pthread_cond_signal(&cond_escrita);

        if (pos_leitura==ultimo_bloco){
            pthread_mutex_unlock(locks+pos_leitura);
            break;
        }

        pthread_mutex_unlock(locks+pos_leitura);

        pos_leitura = (pos_leitura + 1) % tamanho_buffer;
        if (pos_leitura == 0){
            barreira(pos_leitura);
        }
    }
    pthread_exit(NULL);

}

void *padrao_c(void *args){
    /*
     * Busca a quantidade de ocorrências da sequência <012345>.
     */

    int pos_leitura=0, tamanho=tamanho_bloco;

    int aux = -1;

    while(1){

        // Entrada para leitura

        pthread_mutex_lock(locks+pos_leitura);

        while(status_buffer[pos_leitura] == 0){ // Enquanto a linha estiver vazia
            pthread_cond_wait(&cond_leitura, locks+pos_leitura);
        }

        pthread_mutex_unlock(locks+pos_leitura);

        // Leitura

        if (pos_leitura == ultimo_bloco){
            tamanho = tamanho_ultimo_bloco;
        }


        for (int i=0; i<tamanho; i++){
            int sequencia = buffer[pos_leitura*tamanho_bloco+i];

            if (sequencia == aux+1){
                aux = sequencia;
            } else {
                aux = -1;
            }

            if (aux == 5){
                qtd_seq_1_a_5++;
                aux = -1;
            }
        }

        // Saída da leitura

        pthread_mutex_lock(locks+pos_leitura);
        status_buffer[pos_leitura]--;

        if (status_buffer[pos_leitura] == 0) pthread_cond_signal(&cond_escrita);

        if (pos_leitura==ultimo_bloco){
            pthread_mutex_unlock(locks+pos_leitura);
            break;
        }

        pthread_mutex_unlock(locks+pos_leitura);

        pos_leitura = (pos_leitura + 1) % tamanho_buffer;

        if (pos_leitura == 0){
            barreira(pos_leitura);
        }
    }

    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim, tempo_delta;
    pthread_t thread_id[4];
    char *nome_arquivo;

    if (argc < 4){
        fprintf(stderr,"Informe os parâmetros: <nome arquivo> <tamanho do bloco> <tamanho do buffer> para o programa %s\n", argv[0]);
        return 1;
    }

    nome_arquivo = argv[1];
    tamanho_buffer = atoi(argv[2]);
    tamanho_bloco = atoi(argv[3]);

    // Adiciona tomada de tempo

    GET_TIME(tempo_inicio)

    buffer = malloc(sizeof(int) * tamanho_buffer * tamanho_bloco);
    if (!buffer){
        fprintf(stderr, "Erro ao alocar memória para o buffer\n");
        exit(1);
    }

    status_buffer = malloc(sizeof(char) * tamanho_buffer);
    if (!status_buffer){
        fprintf(stderr, "Erro ao alocar memória para o status_buffer\n");
        exit(1);
    }

    for (int i=0; i<tamanho_buffer; i++){
        status_buffer[i] = 0;
    }

    locks = malloc(sizeof(pthread_mutex_t) * tamanho_buffer);
    if (!locks){
        fprintf(stderr, "Erro ao alocar memória para o vetor de locks\n");
        exit(1);
    }

    for (int i=0; i<tamanho_buffer; i++){
        pthread_mutex_init(locks+i, NULL);
    }

    // Inicialização do condicional

    pthread_cond_init(&cond_escrita, NULL);
    pthread_cond_init(&cond_leitura, NULL);

    arquivo = fopen(nome_arquivo, "rb");

    if (!arquivo){
        fprintf(stderr, "Não foi possível abrir o arquivo %s \n", nome_arquivo);
    }

    pthread_create(&thread_id[0], NULL, insere_buffer, NULL);
    pthread_create(&thread_id[1], NULL, padrao_a, NULL);
    pthread_create(&thread_id[2], NULL, padrao_b, NULL);
    pthread_create(&thread_id[3], NULL, padrao_c, NULL);


    for (int i=0; i<4; i++){
        int status_pthread_join = pthread_join(*(thread_id+i), NULL);
        if (status_pthread_join) {
            fprintf(stderr, "Status code retornado no pthread_join() = %d\n", status_pthread_join);
            exit(1);
        }
    }

    fclose(arquivo);

    printf("Maior sequência de valores idênticos: %lld %lld %d\n", valores_identicos.pos_inicial, valores_identicos.tamanho, valores_identicos.valor);
    printf("Quantidade de triplas: %lld \n", qtd_triplas);
    printf("Quantidade de ocorrências da sequência <012345>: %lld \n", qtd_seq_1_a_5);

    // Fim da tomada de tempo

    GET_TIME(tempo_fim)
    tempo_delta = tempo_fim - tempo_inicio;
    printf("Tempo total: %lf\n", tempo_delta);

    return 0;

}