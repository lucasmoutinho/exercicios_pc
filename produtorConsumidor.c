#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define TRUE 1
#define N 10
#define PRODUTOR 15
#define CONSUMIDOR 15

sem_t posicoes_livres;
sem_t posicoes_ocupadas;
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;
pthread_t produtor[PRODUTOR], consumidor[CONSUMIDOR];

int buffer[N];
int count;
int item;

void * produzindo(void * arg) {
  int id = *((int *)arg);

  srand(time(NULL));

  while(1) {
    sem_wait(&posicoes_livres);
    pthread_mutex_lock(&l);
    item = (rand() % 10);
    buffer[count] = item;
    printf("Produtor [%d]: coloquei o item %d na posicao %d do buffer\n", id, item, count);
    sleep(1);
    count++;
    sem_post(&posicoes_ocupadas);
    pthread_mutex_unlock(&l);
    sleep(1);
  }
}

void * consumindo(void * arg) {
  int id = *((int *)arg);

  srand(time(NULL));

  while(1) {
    sem_wait(&posicoes_ocupadas);
    pthread_mutex_lock(&l);
    count--;
    item = buffer[count];
    printf("Consumidor [%d]: consumi o item %d na posicao %d do buffer\n", id, item, count);
    sleep(1);
    buffer[count] = -1;
    sem_post(&posicoes_livres);
    pthread_mutex_unlock(&l);
    sleep(1);
  }
}

int main()
{

  int i;
  int *id;

  count = 0;

  sem_init(&posicoes_livres, 0, N);
  sem_init(&posicoes_ocupadas, 0, 0);

  for (i = 0; i < PRODUTOR; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&produtor[i], NULL, produzindo, (void *)(id));
  }

  for (i = 0; i < CONSUMIDOR; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&consumidor[i], NULL, consumindo, (void *)(id));
  }

  pthread_join(produtor[0], NULL);
  pthread_join(consumidor[0], NULL);

  return 0;
}