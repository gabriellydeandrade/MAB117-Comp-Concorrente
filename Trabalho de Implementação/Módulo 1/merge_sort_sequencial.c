#include<stdio.h>

// Definições das variáveis globais
#define TAM 10
//int vetor[TAM] = {5,3,10,1};
//int vetor[TAM] = {9,1,10,-6,5};
int vetor[TAM] = {9,1,10,-6,5,7,0,8,1,9};

// Protótipos das funções
void imprime_vetor();
void mergeSort(int inicio, int fim);
void merge(int inicio, int meio, int fim);


int main(){
    printf("Vetor antes \n");
    imprime_vetor();

    mergeSort(0, TAM);

    printf("Vetor depois \n");
    imprime_vetor();
}

void mergeSort(int inicio, int fim){
    if (fim - inicio > 1){
        int meio = (inicio+fim)/2;

        mergeSort(inicio, meio);
        mergeSort(meio, fim);

        merge(inicio, meio, fim);
    }
}

void merge(int inicio, int meio, int fim){
    int qtd_elementos_esquerdo = meio - inicio;
    int qtd_elementos_direito = fim - meio;


    int esquerdo[qtd_elementos_esquerdo], direito[qtd_elementos_direito];
    int topo_esquerdo = 0, topo_direito = 0;

    for (int i=0; i<qtd_elementos_esquerdo; i++){
        esquerdo[i] = vetor[inicio + i];
    }

    for (int i=0; i<qtd_elementos_direito; i++){
        direito[i] = vetor[meio + i];
    }

    for (int i=inicio; i<fim; i++){
        if (topo_esquerdo >= qtd_elementos_esquerdo){
            vetor[i] = direito[topo_direito];
            topo_direito++;
        }
        else if (topo_direito >= qtd_elementos_direito){
            vetor[i] = esquerdo[topo_esquerdo];
            topo_esquerdo++;
        }
        else if (esquerdo[topo_esquerdo] < direito[topo_direito]){
            vetor[i] = esquerdo[topo_esquerdo];
            topo_esquerdo++;
        }
        else {
            vetor[i] = direito[topo_direito];
            topo_direito++;
        }
    }

}

void imprime_vetor(){
    for (int i=0; i<TAM; i++){
        printf("%d ", vetor[i]);
    }
    printf("\n");
}