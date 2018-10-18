/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 8 - PROGRAMAÇÃO CONCORRENTE - 2/2018
PROBLEMA DOS FILOSOFOS - FILOSOFOS SO EXISTEM PRA PENSAR E COMER. EM UMA MESA COM 5 FILOSOS, ENTRE CADA UM HA UMA COLHER
PARA COMER O FILOSOFO PRECISA DE USAR UM GARFO NA ESQUERDA E UM NA DIREITA. QUANDO O FILOSOFO PARA DE PENSAR E QUER COMER
ELE PEGA OS DOIS GARFOS E COME, O FILOSOFO ADJACENTE NAO PODE COMER POIS O GARFO ESTA SENDO USADO.

SOLUÇÃO 1 - MENOS OTIMIZADA:

#DEFINE N = 5
#DEFINE ESQ(I) (I + N - 1)%N
#DEFINE DIR(I) (I+1)%N

SEMAFORO S[N] = {1,...,1}
LOCK L;

filosofos(int i){
  while(true){
    // pensam
    .
    .
    .
    lock(l);
    sem_wait(s[esq(i)]);
    sem_wait(s[dir(i)]);
    unlock(l);
    // comem
    sem_post(s[esq(i)]);
    sem_post(s[dir(i)]);
  }
}

SOLUÇÃO 2 - MAIS OTIMIZADA:

#DEFINE N = 5
#DEFINE ESQ(I) (I + N - 1)%N
#DEFINE DIR(I) (I+1)%N

#DEFINE COMENDO 0
#DEFINE FAMINTO 1
#DEFINE PENSANDO 2

int est[N] = {PENSANDO,...,PENSANDO};
LOCK L;

SEMAFORO S[N] = {0,...,0}

teste(int i){
  if(est[i] == FAMINTO && est[esq(i)] != COMENDO && est[dir(i)] != COMENDO){
    est[i] = COMENDO;
    sem_post(s[i])
  }
}

pega_garfos(int i){
  lock(l)
  est[i] = FAMINTO
  teste(i);
  unlock(l)
  sem_wait(s[i]);
}

devolve_garfos(int i){
  lock(l)
  est[i] = PENSANDO
  teste(esq(i));
  teste(dir(i));
  unlock(l)
}


filosofos(int i){
  while(true){
    // pensam
    .
    .
    .
    pega_garfos(i);
    // comem
    devolve_garfos(i);
  }
}

*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define TRUE 1
#define N 5

pthread_t filosofo[N];
sem_t garfo[N];
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;

int dir(int i){
  return (i+1)%N;
}

int esq(int i){
  return (i+N)%N;
}

void *filosofando(void *arg){

  int id = *((int *)arg);

  while(TRUE){
    printf("Filosofo %d está pensando: Ohmmmmmm...\n", id);
    /*PENSANDO*/
    sleep(5);
    printf("Filosofo %d acordou e está com fome\n", id);
    pthread_mutex_lock(&l);
    sem_wait(&garfo[esq(id)]);
    printf("Filosofo %d pegou o garfo %d na esquerda\n", id, esq(id));
    sem_wait(&garfo[dir(id)]);
    printf("Filosofo %d pegou o garfo %d na direita\n", id, dir(id));
    pthread_mutex_unlock(&l);
    printf("Filosofo %d esta comendo: NHAM NHAM!\n", id);
    /*COMENDO*/
    sleep(2);
    printf("Filosofo %d acabou de comer...devolvendo garfos %d e %d\n", id, esq(id), dir(id));
    sem_post(&garfo[esq(id)]);
    sem_post(&garfo[dir(id)]);
    sleep(1);
  }

  pthread_exit(0);
}

int main(){

  int i;
  int *id;

  for(i = 0; i < N; i++){
    sem_init(&garfo[i], 0, 1);
  }

  for (i = 0; i < N; i++){
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&filosofo[i], NULL, filosofando, (void *)(id));
  }

  pthread_join(filosofo[0], NULL);

  return 0;
}