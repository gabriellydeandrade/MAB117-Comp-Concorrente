#include <assert.h>
#include <stdio.h>
#include "merge_sort_concorrente.c"

void testa_se_retorna_vetor_ordenado(){
    int tamanho_vetor = 4;
    int qtd_threads = 1;
    int vetor[] = {10, 5, 1, 0};

    int *resultado = mergeSort(tamanho_vetor, vetor, qtd_threads);

    int vetor_esperado[] = {0, 1, 5, 10};

    int count = 0;
    for (int i=0; i<tamanho_vetor; i++){
        if (resultado[i] == vetor_esperado[i]) {
            count ++;
        }
    }

    assert(count == tamanho_vetor);
    printf("Teste `testa_se_retorna_vetor_ordenado` passou \n");
}

int teste() {
    testa_se_retorna_vetor_ordenado();
    return 0;
}