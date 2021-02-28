#include<stdio.h>

int main () {
    FILE *fp;
    long long int tamanho_arquivo = 30;
    int sequencia[] = {1,2,3,1,1,1,1,2,3,4,5,5,5,5,5,5,5,0,0,0,3,3,3,0,1,2,3,4,5,0};
    int valor_lido;

    // Gravando arquivo

    fp = fopen( "arquivo" , "wb" );

    fwrite(&tamanho_arquivo, sizeof(long long int) , 1,  fp );
    for (int n=0; n<31; n++){
        fwrite(&sequencia[n], sizeof(int) , 1,  fp );
    }

    fclose(fp);

    // Lendo arquivo

    fp = fopen( "arquivo" , "rb" );
    for (int n=0; n<31; n++){
        fread(&valor_lido, sizeof(int) , 1,  fp );
        printf("Valor lido: %d \n", valor_lido);
    }


    return(0);
}