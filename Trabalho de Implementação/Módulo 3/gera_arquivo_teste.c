#include<stdio.h>
#include<stdlib.h>
#include "time.h"

int *inicializa_vetor_randomico(long long int tamanho_arquivo_randomico){
    time_t t;

    // Inicializa a semente randômica que iremos usar na inicialização
    srand((unsigned) time(&t));

    int *vetor_randomico = malloc(tamanho_arquivo_randomico * sizeof(int));
    if(!vetor_randomico){
        fprintf(stderr, "Não foi possível alocar memória para o vetor_randomico usando o malloc \n");
        exit(1);
    }

    for (int i = 0; i < tamanho_arquivo_randomico; i++)
        vetor_randomico[i] = rand() % 6;

    return vetor_randomico;
}

int main () {
    FILE *fp;
    long long int tamanho_arquivo = 30;
    int sequencia[] = {1,2,3,1,1,1,1,2,3,4,5,5,5,5,5,5,5,0,0,0,3,3,3,0,1,2,3,4,5,0};
    int valor_lido;

    // Modo arquivo com 30 valores

    // Gravando arquivo

    fp = fopen( "arquivo" , "wb" );

    fwrite(&tamanho_arquivo, sizeof(long long int) , 1,  fp );
    for (int n=0; n<tamanho_arquivo+1; n++){
        fwrite(&sequencia[n], sizeof(int) , 1,  fp );
    }

    fclose(fp);

    // Lendo arquivo

//    fp = fopen( "arquivo" , "rb" );
//    for (int n=0; n<31; n++){
//        fread(&valor_lido, sizeof(int) , 1,  fp );
//        printf("Valor lido: %d \n", valor_lido);
//    }

    // Modo randomico (feito para arquivos grandes)

    long long int tamanho_arquivo_randomico = 1000000000;
    int *vetor_randomico = inicializa_vetor_randomico(tamanho_arquivo_randomico);

    // Gravando arquivo

    fp = fopen( "arquivo_randomico_10_a_9" , "wb" );

    fwrite(&tamanho_arquivo_randomico, sizeof(long long int) , 1,  fp );
    for (int n=0; n<tamanho_arquivo_randomico+1; n++){
        fwrite(&vetor_randomico[n], sizeof(int) , 1,  fp );
    }

    fclose(fp);

    return(0);
}