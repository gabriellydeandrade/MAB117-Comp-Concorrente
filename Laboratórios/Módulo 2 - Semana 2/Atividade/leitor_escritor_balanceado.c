/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Laboratório: 2 */
/* Laboratório: implementa  o problema dos leitores/escritores com diferentes abordagens de prioridade
 * escritor e leitor balanceados (sem inanição)
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NTHREADS_LEITORA 6 //numero de threads leitoras
#define NTHREADS_ESCRITORA 6 //numero de threads escritoras
#define TAM_VETOR 5

//variaveis do problema
int *vetor;
int qtd_tarefas_lendo=0; //contador de threads lendo
int qtd_tarefas_escrevendo=0; //contador de threads escrevendo
int tamanho_fila_escrita=0; //contador de escritores na fila de prioridade
int tamanho_fila_leitura=0; //contador de leitores na fila de prioridade
int qtd_leitores_podem_ler=0;
int vez_de_quem=0; // 0 para leitura e 1 para escrita

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t condicao_leitura, condicao_escrita;

void inicio_leitura(int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);

    tamanho_fila_leitura++;
    while((vez_de_quem == 1 && tamanho_fila_escrita > 0) || qtd_tarefas_escrevendo > 0){
        if (vez_de_quem == 1 && tamanho_fila_escrita > 0){
            printf("L[%d] bloqueou. Era vez da %s\n", id, vez_de_quem ? "escrita" : "leitura");
        }
        else{
            printf("L[%d] bloqueou. Existe alguém escrevendo. \n", id);
        }
        pthread_cond_wait(&condicao_leitura, &mutex);
        printf("L[%d] desbloqueou\n", id);
    }
    tamanho_fila_leitura--;
    qtd_leitores_podem_ler=qtd_tarefas_lendo == 0 ? tamanho_fila_leitura : qtd_leitores_podem_ler-1;
    vez_de_quem = 1;
    qtd_tarefas_lendo++;
    pthread_mutex_unlock(&mutex);
}

void fim_leitura(int id) {
    pthread_mutex_lock(&mutex);
    printf("L[%d] terminou de ler\n", id);

    qtd_tarefas_lendo--;
    if(qtd_tarefas_lendo==0) pthread_cond_signal(&condicao_escrita);
    pthread_mutex_unlock(&mutex);
}

void inicio_escrita(int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);

    tamanho_fila_escrita++;
    while((vez_de_quem == 0 && tamanho_fila_leitura > 0) || qtd_tarefas_lendo>0 || qtd_tarefas_escrevendo>0) {
        if (vez_de_quem == 0 && tamanho_fila_leitura > 0){
            printf("E[%d] bloqueou. Vez de %s\n", id, vez_de_quem ? "escrita" : "leitura");
        }
        else if (qtd_tarefas_lendo>0){
            printf("E[%d] bloqueou. Existe%c %d tarefa%c lendo. \n",
                   id, qtd_tarefas_lendo > 1 ? 'm' : 0, qtd_tarefas_lendo, qtd_tarefas_lendo > 1 ? 's' : 0);
        }
        else {
            printf("E[%d] bloqueou. Existe alguém escrevendo. \n", id);
        }
        pthread_cond_wait(&condicao_escrita, &mutex);
        printf("E[%d] desbloqueou\n", id);
    }
    tamanho_fila_escrita--;
    vez_de_quem = 0;
    qtd_tarefas_escrevendo++;
    pthread_mutex_unlock(&mutex);
}

void fim_escrita(int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] terminou de escrever\n", id);

    qtd_tarefas_escrevendo--;
    pthread_cond_signal(&condicao_escrita);
    pthread_cond_broadcast(&condicao_leitura);
    pthread_mutex_unlock(&mutex);
}

void * leitor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        inicio_leitura(*id);
        printf("Leitora %d esta lendo\n", *id);

        double total = 0;
        char buffer[20 * (TAM_VETOR + 2)] = "";
        char *ponteiro_buffer = buffer;

        sprintf(ponteiro_buffer, "L[%d] == vetor lido == ", *id);
        for (int i=0; i < TAM_VETOR; i++){
            sprintf(ponteiro_buffer, "%s%d ", buffer,vetor[i]);
            total += vetor[i];
        }
        printf("%s == média == %.2f \n", buffer, total / TAM_VETOR);

        fim_leitura(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

void * escritor(void * arg) {
    int *id = (int *) arg;
    while(1) {
        inicio_escrita(*id);
        printf("Escritora %d esta escrevendo\n", *id);

        vetor[0] = *id;
        vetor[TAM_VETOR-1] = *id;
        for (int i=1; i < TAM_VETOR-1; i++){
            vetor[i] = *id*2;
        }

        fim_escrita(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(NULL);
}

void preenche_vetor(){
    time_t t;

    // Inicializa a semente randômica que iremos usar na inicialização
    srand((unsigned) time(&t));

    vetor = malloc(TAM_VETOR * sizeof(int));
    if(!vetor){
        fprintf(stderr, "Não foi possível alocar memória para o vetor usando o malloc \n");
        exit(1);
    }

    for (int i = 0; i < TAM_VETOR; i++)
        vetor[i] = rand() % 100;
}

int main(void) {
    //identificadores das threads
    pthread_t tid[NTHREADS_LEITORA+NTHREADS_ESCRITORA];
    int id[NTHREADS_LEITORA+NTHREADS_ESCRITORA];

    //inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condicao_leitura, NULL);
    pthread_cond_init(&condicao_escrita, NULL);

    preenche_vetor();

    //cria as threads leitoras
    for(int i=0; i<NTHREADS_LEITORA; i++) {
        id[i] = i+1;
        if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
    }

    //cria as threads escritoras
    for(int i=0; i<NTHREADS_ESCRITORA; i++) {
        id[i+NTHREADS_LEITORA] = i+1;
        if(pthread_create(&tid[i+NTHREADS_LEITORA], NULL, escritor, (void *) &id[i+NTHREADS_LEITORA])) exit(-1);
    }

    pthread_exit(NULL);
    return 0;
}
