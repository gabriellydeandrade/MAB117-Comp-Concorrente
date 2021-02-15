#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NTHREADS_LEITORA 2
#define NTHREADS_ESCRITORA 2

pthread_mutex_t em_escrita, em_leitura;
sem_t cond_escrita, cond_leitura;
int qtd_leitores=0, qtd_escritores=0;

void *leitor(void * arg){
    int *id = (int *) arg;
    while(1){
        printf("L[%d] quer ler\n", *id);

        sem_wait(&cond_leitura);
        pthread_mutex_lock(&em_leitura); qtd_leitores++;
        if (qtd_leitores == 1) sem_wait(&cond_escrita);
        pthread_mutex_unlock(&em_leitura);
        sem_post(&cond_leitura);

        // Inicio da leitura
        printf("L[%d] está lendo\n", *id);
        sleep(1);

        pthread_mutex_lock(&em_leitura); qtd_leitores--;
        if (qtd_leitores == 0) sem_post(&cond_escrita);
        pthread_mutex_unlock(&em_leitura);

        printf("L[%d] terminou de ler\n", *id);
    }

}

void *escritor(void * arg){
    int *id = (int *) arg;
    while(1){
        printf("E[%d] quer escrever\n", *id);

        pthread_mutex_lock(&em_escrita); qtd_escritores++;
        if (qtd_escritores == 1) sem_wait(&cond_leitura);
        pthread_mutex_unlock(&em_escrita);
        sem_wait(&cond_escrita);

        // Inicio da escrita
        printf("E[%d] está escrevendo\n", *id);
        sleep(2);

        sem_post(&cond_escrita);
        pthread_mutex_lock(&em_escrita); qtd_escritores--;
        if (qtd_escritores == 0) sem_post(&cond_leitura);
        pthread_mutex_unlock(&em_escrita);

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

    //inicia os semaforos
    sem_init(&cond_escrita, 0, 1); //condição
    sem_init(&cond_leitura, 0, 1); //condição

    pthread_exit(NULL);
    return 0;


}