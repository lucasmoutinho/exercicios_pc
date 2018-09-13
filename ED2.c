/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 2 - PROGRAMAÇÃO CONCORRENTE - 2/2018
PROBLEMA DOS LEITORES E ESCRITORES - QUEREM LER O BD AO MESMO TEMPO, QUANDO LEITORES ESTÃO LENDO, VARIOS LEITORES
PODEM LER AO MESMO TEMPO, MAS NÃO PERMITEM ESCREVER. QUANDO UM ESCRITOR ESTA ESCREVENDO, SO ELE PODE ESCREVER.

*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define NUM_ESCRITORES 5
#define NUM_LEITORES 10

pthread_mutex_t lock_l = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_bd = PTHREAD_MUTEX_INITIALIZER;
int n = 0;


void *leitura(void * arg){
	int id = *((int *) arg);

    while (TRUE) {
        pthread_mutex_lock(&lock_l);
        n++;
        if(n==1) {
        pthread_mutex_lock(&lock_bd);
        }
        pthread_mutex_unlock(&lock_l);
        printf("--Leitor%d: Estou lendo o BD\n", id);
        sleep(3);
        printf("--Leitor%d: Terminei de ler o BD\n", id);
        pthread_mutex_lock(&lock_l);
        n--;
        if(n==0) {
        pthread_mutex_unlock(&lock_bd);
        }
        pthread_mutex_unlock(&lock_l);
        sleep(3);
    }

}

void *escrita(void * arg){
  int id = *((int *) arg);

    while (TRUE) {
        sleep(3);
        pthread_mutex_lock(&lock_bd);
        printf("--Escritor%d: Estou escrevendo no BD\n", id);
        sleep(3);
        printf("--Escritor%d: Terminei de escrever no BD\n", id);
        pthread_mutex_unlock(&lock_bd);
    }

}

int main(){

	pthread_t w[NUM_ESCRITORES], r[NUM_LEITORES];
	int i;
    int *id;

    for (i = 0; i < NUM_LEITORES ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&r[i], NULL, leitura, (void *) (id));
    }
    for (i = 0; i < NUM_ESCRITORES ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&w[i], NULL, escrita, (void *) (id));
    }

    pthread_join(r[0],NULL);
    pthread_join(w[0],NULL);

    return 0;
}