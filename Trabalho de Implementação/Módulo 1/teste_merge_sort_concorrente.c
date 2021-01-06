#include <assert.h>
#include <stdio.h>
#include "merge_sort_concorrente.c"

#define ANSI_COR_FALHA     "\x1b[31m"
#define ANSI_COR_SUCESSO   "\x1b[32m"
#define ANSI_COR_PADRAO   "\x1b[0m"

// Define variáveis globais para os testes que serão modificadas para cada teste unitário
int tamanho_vetor;
int qtd_threads;

int vetores_iguais(int *vetor_resultado, int *vetor_esperado){
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
    tamanho_vetor = 4;
    qtd_threads = 1;
    int vetor[] = {10, 5, 1, 0};
    int vetor_esperado[] = {0, 1, 5, 10};

    int *resultado = merge_sort(tamanho_vetor, vetor, qtd_threads);


    if (vetores_iguais(resultado, vetor_esperado))
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
    tamanho_vetor = 4;
    qtd_threads = 1;
    int vetor[] = {0, 1, 5, 10};
    int vetor_esperado[] = {0, 1, 5, 10};

    int *resultado = merge_sort(tamanho_vetor, vetor, qtd_threads);

    if (vetores_iguais(resultado, vetor_esperado))
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
    tamanho_vetor = 11;
    qtd_threads = 1;
    int vetor[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int vetor_esperado[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int *resultado = merge_sort(tamanho_vetor, vetor, qtd_threads);

    if (vetores_iguais(resultado, vetor_esperado))
        printf(ANSI_COR_SUCESSO "Teste `testa_se_ordena_vetor_ordenado_em_ordem_decrescente` passou \n" ANSI_COR_PADRAO);
    else{
        printf(ANSI_COR_FALHA "Teste `testa_se_ordena_vetor_ordenado_em_ordem_decrescente` falhou \n" ANSI_COR_PADRAO);

        puts(ANSI_COR_FALHA "VETOR ESPERADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, vetor_esperado);

        puts(ANSI_COR_FALHA"RESULTADO" ANSI_COR_PADRAO);
        imprime_vetor(tamanho_vetor, resultado);
    }



}

int teste() {
    testa_se_retorna_vetor_ordenado();
    testa_se_retorna_vetor_igual_se_ele_ja_for_ordenado();
    testa_se_ordena_vetor_ordenado_em_ordem_decrescente();

    return 0;
}