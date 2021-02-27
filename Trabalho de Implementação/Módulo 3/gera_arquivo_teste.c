#include<stdio.h>

int main () {
    FILE *fp;
    int sequencia[] = {30,1,2,3,1,1,1,1,2,3,4,5,5,5,5,5,5,5,0,0,0,3,3,3,0,1,2,3,4,5,0};
    int valor_lido;

    // Gravando arquivo

    fp = fopen( "arquivo.txt" , "wb" );

    for (int n=0; n<31; n++){
        fwrite(&sequencia[n], sizeof(int) , 1,  fp );
    }

    fclose(fp);

    // Lendo arquivo

    fp = fopen( "arquivo.txt" , "rb" );
    for (int n=0; n<31; n++){
        fread(&valor_lido, sizeof(int) , 1,  fp );
        printf("Valor lido: %d \n", valor_lido);
    }


    return(0);
}