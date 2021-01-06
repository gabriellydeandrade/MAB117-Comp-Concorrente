#include <assert.h>
#include <stdio.h>
#include "main.h"

#define ANSI_COR_FALHA     "\x1b[31m"
#define ANSI_COR_SUCESSO   "\x1b[32m"
#define ANSI_COR_PADRAO   "\x1b[0m"


int vetores_iguais(int *vetor_resultado, int *vetor_esperado, int tamanho_vetor){
    int qtd_elementos_iguais = 0;
    for (int i=0; i<tamanho_vetor; i++){
        if (vetor_resultado[i] == vetor_esperado[i]) {
            qtd_elementos_iguais++;
        }
    }

    if (qtd_elementos_iguais == tamanho_vetor)
        return 1;
    else
        return 0;

}

void testa_se_retorna_vetor_ordenado(){
    int tamanho_vetor = 4;
    int qtd_threads = 1;
    int vetor[] = {10, 5, 1, 0};
    int vetor_esperado[] = {0, 1, 5, 10};

    int *resultado = merge_sort(tamanho_vetor, vetor, qtd_threads);


    if (vetores_iguais(resultado, vetor_esperado, tamanho_vetor))
        printf(ANSI_COR_SUCESSO "Teste `testa_se_retorna_vetor_ordenado` passou \n" ANSI_COR_PADRAO);
    else{
        printf(ANSI_COR_FALHA "Teste `testa_se_retorna_vetor_ordenado` falhou \n" ANSI_COR_PADRAO);

        puts(ANSI_COR_FALHA "VETOR ESPERADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, vetor_esperado);

        puts(ANSI_COR_FALHA"RESULTADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, resultado);
    }
}

void testa_se_retorna_vetor_igual_se_ele_ja_for_ordenado(){
    int tamanho_vetor = 4;
    int qtd_threads = 1;
    int vetor[] = {0, 1, 5, 10};
    int vetor_esperado[] = {0, 1, 5, 10};

    int *resultado = merge_sort(tamanho_vetor, vetor, qtd_threads);

    if (vetores_iguais(resultado, vetor_esperado, tamanho_vetor))
        printf(ANSI_COR_SUCESSO "Teste `testa_se_retorna_vetor_igual_se_ele_ja_for_ordenado` passou \n" ANSI_COR_PADRAO);
    else{
        printf(ANSI_COR_FALHA "Teste `testa_se_retorna_vetor_igual_se_ele_ja_for_ordenado` falhou \n" ANSI_COR_PADRAO);

        puts(ANSI_COR_FALHA "VETOR ESPERADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, vetor_esperado);

        puts(ANSI_COR_FALHA"RESULTADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, resultado);
    }

}

void testa_se_ordena_vetor_ordenado_em_ordem_decrescente(){
    int tamanho_vetor = 11;
    int qtd_threads = 1;
    int vetor[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int vetor_esperado[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int *resultado = merge_sort(tamanho_vetor, vetor, qtd_threads);

    if (vetores_iguais(resultado, vetor_esperado, tamanho_vetor))
        printf(ANSI_COR_SUCESSO "Teste `testa_se_ordena_vetor_ordenado_em_ordem_decrescente` passou \n" ANSI_COR_PADRAO);
    else{
        printf(ANSI_COR_FALHA "Teste `testa_se_ordena_vetor_ordenado_em_ordem_decrescente` falhou \n" ANSI_COR_PADRAO);

        puts(ANSI_COR_FALHA "VETOR ESPERADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, vetor_esperado);

        puts(ANSI_COR_FALHA"RESULTADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, resultado);
    }
}

void testa_se_ordena_corretamente_independente_da_quantidade_de_threads(){
    int tamanho_vetor = 10;
    int vetor_esperado[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int qtd_threads=1; qtd_threads<tamanho_vetor; qtd_threads++){
        int vetor[] = {5, 3, 9, 1, 0, 2, 6, 7, 4, 8};
        int *resultado = merge_sort(tamanho_vetor, vetor, qtd_threads);

        if (vetores_iguais(resultado, vetor_esperado, tamanho_vetor))
            printf(ANSI_COR_SUCESSO "Teste `testa_se_ordena_corretamente_independente_da_quantidade_de_threads` com %d thread(s) passou \n" ANSI_COR_PADRAO, qtd_threads);
        else{
            printf(ANSI_COR_FALHA "Teste `testa_se_ordena_corretamente_independente_da_quantidade_de_threads` com %d thread(s)  falhou \n" ANSI_COR_PADRAO, qtd_threads);

            puts(ANSI_COR_FALHA "VETOR ESPERADO" ANSI_COR_PADRAO);
            imprime_vetor(tamanho_vetor, vetor_esperado);

            puts(ANSI_COR_FALHA"RESULTADO" ANSI_COR_PADRAO);
            imprime_vetor(tamanho_vetor, resultado);

            printf(ANSI_COR_FALHA "QTD DE THREADS: %d \n", qtd_threads);
        }
    }
}

void teste() {
    testa_se_retorna_vetor_ordenado();
    testa_se_retorna_vetor_igual_se_ele_ja_for_ordenado();
    testa_se_ordena_vetor_ordenado_em_ordem_decrescente();
    testa_se_ordena_corretamente_independente_da_quantidade_de_threads();
}