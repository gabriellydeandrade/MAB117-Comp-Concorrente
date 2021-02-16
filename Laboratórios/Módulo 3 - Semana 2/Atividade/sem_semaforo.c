#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NTHREADS_LEITORA 2
#define NTHREADS_ESCRITORA 1

pthread_mutex_t mutex;
pthread_cond_t cond_escrita_lock, cond_leitura_lock;
int qtd_leitores=0, qtd_escritores=0, fila_escrita=0;

void *leitor(void * arg){
    int *id = (int *) arg;
    while(1){
        printf("L[%d] quer ler\n", *id);

        pthread_mutex_lock(&mutex);
        while (fila_escrita > 0 || qtd_escritores > 0) {
            printf("L[%d] bloqueou: %d na fila e %d escritores\n", *id, fila_escrita, qtd_escritores);
            pthread_cond_wait(&cond_leitura_lock, &mutex);
            printf("L[%d] desbloqueou: %d na fila e %d escritores\n", *id, fila_escrita, qtd_escritores);
        }

        qtd_leitores++;
        pthread_mutex_unlock(&mutex);

        // Inicio da leitura
        printf("L[%d] está lendo\n", *id);
        sleep(1);

        pthread_mutex_lock(&mutex); qtd_leitores--;
        if (qtd_leitores == 0) {
            pthread_cond_signal(&cond_escrita_lock);
        }
        pthread_mutex_unlock(&mutex);

        printf("L[%d] terminou de ler\n", *id);
    }

}

void *escritor(void * arg){
    int *id = (int *) arg;
    while(1){
        printf("E[%d] quer escrever\n", *id);

        pthread_mutex_lock(&mutex);
        fila_escrita++;
        while (qtd_leitores > 0 || qtd_escritores > 0) {
            printf("E[%d] bloqueou: %d escritores e %d leitores\n", *id, qtd_escritores, qtd_leitores);
            pthread_cond_wait(&cond_escrita_lock, &mutex);
            printf("E[%d] desbloqueou: %d escritores e %d leitores\n", *id, qtd_escritores, qtd_leitores);
        }

        qtd_escritores++;
        pthread_mutex_unlock(&mutex);

        // Inicio da escrita
        printf("E[%d] está escrevendo\n", *id);
        sleep(2);

        pthread_mutex_lock(&mutex);
        fila_escrita--;
        qtd_escritores--;
        if (fila_escrita == 0) pthread_cond_broadcast(&cond_leitura_lock);
        pthread_mutex_unlock(&mutex);

        printf("E[%d] terminou de escrever\n", *id);
    }



}

int main(void){
    pthread_t tid[NTHREADS_LEITORA+NTHREADS_ESCRITORA];
    int id[NTHREADS_LEITORA+NTHREADS_ESCRITORA];

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

    //inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitura_lock, NULL);
    pthread_cond_init(&cond_escrita_lock, NULL);

    pthread_exit(NULL);
    return 0;


}