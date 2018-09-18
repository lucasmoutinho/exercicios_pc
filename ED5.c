/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 5 - PROGRAMAÇÃO CONCORRENTE - 2/2018
PROBLEMA DOS PRODUTORES E CONSUMIDORES - O PRODUTOR TEM UM BUFFER LIMITADO DE TAMANHO N ONDE PRODUZ DADOS ( NO CASO, INTEIROS ), 
QUANDO ELE ENCHE O BUFFER ELE DA UM SLEEP E ACORDA O CONSUMIDOR. QUANDO O BUFFER TA VAZIO, O CONSUMIDOR DA UM SLEEP E ACORDA O PRODUTOR
O TEMPO PRA PRODUZIR E CONSUMIR UM INTEIRO PODE SER UM SLEEP

*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define PRODUTOR 10
#define CONSUMIDOR 10
#define TAMANHO 5

pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER; /* lock pro contador*/
pthread_cond_t cp = PTHREAD_COND_INITIALIZER;  /*condicional do produtor*/
pthread_cond_t cc = PTHREAD_COND_INITIALIZER;  /*condicional do consumidor*/
int count = 0;
int item = 0;
int buffer[TAMANHO];

void inicializaBuffer()
{
  int i = 0;
  for (i = 0; i < TAMANHO; i++)
  {
    buffer[i] = -1;
  }
}

void showBuffer()
{
  int i = 0;
  printf("-------------------\n");
  printf("BUFFER: ");
  while (i < TAMANHO && buffer[i] != -1)
  {
    printf("%d ", buffer[i]);
    i++;
  }
  printf("\n-------------------\n");
}

void *produzindo(void *arg)
{
  int id = *((int *)arg);

  while (TRUE)
  {
    pthread_mutex_lock(&l);
    item = (rand() % 10);

    while (count == TAMANHO)
    {
      printf("Produtor %d: O Buffer está cheio, irei dormir\n\n", id);
      pthread_cond_wait(&cp, &l);
    }

    printf("Produtor %d: Estou produzindo o item (%d) para o buffer\n", id, item);
    sleep(1);
    buffer[count] = item;
    count++;
    showBuffer();

    if (count == 1)
    {
      printf("Produtor %d: Comecei a encher o buffer, acordando o consumidor\n\n", id);
      pthread_cond_broadcast(&cc);
    }

    pthread_mutex_unlock(&l);
    printf("\n");
    sleep(rand() % 10);
  }
  pthread_exit(0);
}

void *consumindo(void *arg)
{
  int id = *((int *)arg);

  while (TRUE)
  {
    pthread_mutex_lock(&l);
    item = (rand() % 10);

    while (count == 0)
    {
      printf("Consumidor %d: O Buffer está vazio, irei dormir\n\n", id);
      pthread_cond_wait(&cc, &l);
    }

    count--;
    printf("Consumidor %d: Estou consumindo o item (%d) do buffer\n", id, buffer[count]);
    sleep(1);
    buffer[count] = -1;
    showBuffer();

    if (count == TAMANHO - 1)
    {
      printf("Consumidor %d: Buffer não está mais lotado, acordando o produtor\n\n", id);
      pthread_cond_broadcast(&cp);
    }

    pthread_mutex_unlock(&l);
    printf("\n");
    sleep(rand() % 10);
  }
  pthread_exit(0);
}

int main()
{

  pthread_t produtor[PRODUTOR], consumidor[CONSUMIDOR];
  int i;
  int *id;

  inicializaBuffer();

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
