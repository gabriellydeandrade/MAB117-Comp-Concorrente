# Quick Sort Concorrente

## Objetivo

Implementação de ordenação de vetor pelo método Quick Sort de modo concorrente

## Compilação 

    gcc -o main main.c quicksort_concorrente.c teste_quicksort_concorrente.c -Wall -pthread

## Utilização

Para ordenar um vetor, é possível passar um arquivo, chamar a função diretamente e executar em modo
randômico.

Execute a função **./main** com as seguintes obções de flags e parâmetros:

- Flag "--teste": poderá receber opcionalmente a flag para rodar os testes unitários.
- Flag "--arquivo": permite enviar um arquivo para o vetor ser ordenado.
                  Na primeira linha encontra-se a quantidade de elementos e a quantidade de threads separados por espaço.
                  Na segunda, encontra-se os elementos do vetor separados por espaço.
- Flag "--output": exibe o resultado da ordenação no console.
- Flag "--debug": exibe resultados de log como vetor antes e depois e o tempo da ordenação.
- Flag "--sumario": exibe o tempo que levou para ordenar.
- Flag "--validar": realiza um teste de caixa preta da ordenação e exibe uma mensagem se foi ordenado correto ou não.
- Flag "--tamanho_vetor": recebe o tamanho do vetor a ser ordenado.
- Flag "--threads": recebe a quantidade de threads. Caso não seja definida o padrão é 1 thread (simulando comportamento sequencial).

### Exemplos

1. Utilização de ordenação no modo randômico

   `./main --tamanho_vetor 10 --randomico --output`

2. Utilização de ordenação no modo randômico com validação

   `./main --tamanho_vetor 10 --randomico --output --validar`

3. Utilização de ordenação no modo randômico com 2 threads e modo debug
   
   `./main --tamanho_vetor 10  --threads 2 --randomico --debug`

4. Utilização de ordenação no modo arquivo

   `./main --arquivo arquivo_valido.txt --output`

## Testes

Execute os testes, após compilado, com a flag **--testes**

    ./main --testes