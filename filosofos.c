#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define TRUE 1
#define N 5
#define PENSANDO 0
#define FAMINTO 1
#define COMENDO 2
int estado[N];

pthread_t filosofo[N];
sem_t posicao[N];
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;

int dir(int i)
{
  return (i + 1) % N;
}

int esq(int i)
{
  return (i + N - 1) % N;
}
void teste(int id) {
  if(estado[id] == FAMINTO && estado[esq(id)] != COMENDO && estado[dir(id)] != COMENDO) {
    estado[id] = COMENDO;
    sem_post(&posicao[id]);
  } 
}

void pega_garfo(int id) {
  pthread_mutex_lock(&l);
  estado[id] = FAMINTO;
  printf("FILOSOFO[%d] quero comer\n", id);
  teste(id);
  pthread_mutex_unlock(&l);
  sem_wait(&posicao[id]);
}

void libera_garfo(int id) {
  pthread_mutex_lock(&l);
  estado[id] = PENSANDO;
  printf("filosofo[%d] terminou de comer\n", id);
  teste(esq(id));
  teste(dir(id));
  pthread_mutex_unlock(&l);

}
void *filosofando(void *arg)
{

  int id = *((int *)arg);

  
  while(1){
    printf("FILOSOFO[%d] PENSANDO\n", id);
    sleep(5);
    pega_garfo(id);
    printf("filosofo[%d] comendo\n", id);
    sleep(6);
    libera_garfo(id);

  }
  pthread_exit(0);
}

int main(){

  int i;
  int *id;

  
  for(size_t i = 0; i < N; i++)
  {
    estado[i] = PENSANDO;
  }
  
  for(i = 0; i < N; i++){
    sem_init(&posicao[i], 0, 0);
  }

  for (i = 0; i < N; i++){
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&filosofo[i], NULL, filosofando, (void *)(id));
  }

  pthread_join(filosofo[0], NULL);

  return 0;
}