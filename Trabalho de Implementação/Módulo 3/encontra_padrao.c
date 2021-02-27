#include<stdio.h>

int main(){
    int sequencia[] = {30, 1,2,3,1,1,1,1,2,3,4,5,5,5,5,5,5,5,0,0,0,3,3,3,0,1,2,3,4,5,0};
//    int sequencia[] = {1,2,3,0,1,2,3,4,5,4,5,5,5,5,5,5,5,0,0,0,3,3,3,0,1,2,3,4,5,0};

    // Padrão (a)

    int pos_inicial=0, tamanho=0, valor=0;
    int pos_temp, tamanho_temp=0, valor_temp;

    for (int i=1; i<sequencia[0]; i++){
        if (tamanho_temp == 0){
            valor_temp = sequencia[i];
            pos_temp = i;
        }

        if (sequencia[i] == valor_temp){
            tamanho_temp++;
        } else {
            if (tamanho_temp > tamanho){
                pos_inicial = pos_temp;
                tamanho = tamanho_temp;
                valor = valor_temp;
            }

            tamanho_temp = 0;
        }
    }

    printf("Maior sequẽncia de valores idênticos: %d %d %d\n", pos_inicial, tamanho, valor);


    // Padrão (b)

    int qtd_triplas = 0;
    int numero;
    int contador_padrao_b = 0;

    for (int i=1; i<sequencia[0]; i++){
        if (contador_padrao_b == 0){
            numero = sequencia[i];
        }

        if (sequencia[i] == numero){
            contador_padrao_b++;
        } else {
            contador_padrao_b = 0;
        }

        if (contador_padrao_b == 3){
            qtd_triplas++;
            contador_padrao_b = 0;
        }

    }

    printf("Quantidade de triplas: %d \n", qtd_triplas);


    // Padrão (c)

    int qtd_seq_1_a_5 = 0;
    int aux = -1;

    for (int i=1; i<sequencia[0]; i++){
        if (sequencia[i] == aux+1){
            aux = sequencia[i];
        } else {
            aux = -1;
        }

        if (aux == 5){
            qtd_seq_1_a_5++;
            aux = -1;
        }
    }

    printf("Quantidade de ocorrências da sequência <012345>: %d \n", qtd_seq_1_a_5);

    return 0;
}