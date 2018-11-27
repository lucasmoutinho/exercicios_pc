#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define TRUE 1
#define CADEIRAS 5
#define BARBEIROS 1
#define CLIENTES 15

pthread_t cliente[CLIENTES], barbeiro[BARBEIROS];
sem_t cadeira, cadeira_barbeiro, barb, cortando;

void *barbeiros(void *arg)
{
  int id = *((int *)arg);

  while(TRUE){
    printf("Barbeiro [%d]: zzzzZZZZ\n",id);
    sem_wait(&barb);
    printf("Cortando cabelo..\n");
    sleep(7);
    sem_post(&cortando);
    printf("Cabou o corte. R$ 50,00.\n");
  }

}

void *clientes(void *arg)
{
  int id = *((int *)arg);
  while (TRUE)
  {
    printf("Cliente [%d]: Quero entrar na loja!\n", id);
    if (sem_trywait(&cadeira) == 0){
      printf("Cliente [%d]: Entrei na loja...\n",id);
      sleep(2);
      sem_wait(&cadeira_barbeiro);
      printf("Cliente [%d]: Peguei a cadeira do barbeiro, acordando barb...\n",id);
      sleep(2);
      sem_post(&cadeira);
      sem_post(&barb);
      printf("Cliente [%d]: Cortando cabelo...\n",id);
      sem_wait(&cortando);
      sem_post(&cadeira_barbeiro);
    }
    printf("Cliente [%d]: Vazei rapeize\n", id);
    sleep(2);
  }
}

int main()
{

  int i;
  int *id;

  sem_init(&cadeira, 0, CADEIRAS);
  sem_init(&cadeira_barbeiro, 0, 1);
  sem_init(&cortando, 0, 0);
  sem_init(&barb, 0, 0);

  for (i = 0; i < CLIENTES; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&cliente[i], NULL, clientes, (void *)(id));
  }

  for (i = 0; i < BARBEIROS; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&barbeiro[i], NULL, barbeiros, (void *)(id));
  }

  pthread_join(cliente[0], NULL);
  pthread_join(barbeiro[0], NULL);

  return 0;
}