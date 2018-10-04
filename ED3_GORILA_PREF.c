#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QTD_MACACOS_A 15
#define QTD_MACACOS_B 15
#define QTD_GORILAS 1
#define VERDADEIRO 1

pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entrada = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_a = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_b = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_g = PTHREAD_COND_INITIALIZER;

int qtd_a = 0;
int qtd_b = 0;
int gorila_esperando = 0;

void *lado_gorila(void *id)
{
  int i = *((int *)id);
  while (VERDADEIRO)
  {
    printf("Gorila chegou\n");
    gorila_esperando = 1;
    pthread_mutex_lock(&turno);

    printf("Gorila %d passando...\n", i);
    sleep(5);
    printf("Gorila %d passou!!!!\n", i);
    pthread_cond_broadcast(&cond_a);
    pthread_cond_broadcast(&cond_b);
    gorila_esperando = 0;
    pthread_mutex_unlock(&turno);
    printf("\n\n");
    sleep(5);
  }
}

void *lado_a(void *id)
{
  int i = *((int *)id);
  while (VERDADEIRO)
  {
    pthread_mutex_lock(&entrada);
    pthread_mutex_lock(&turno);
    while(qtd_b > 0 || gorila_esperando )
    {
      printf("Macaco_a %d na espera\n", i);
      pthread_cond_wait(&cond_a,&turno);
    }
    qtd_a++;
    printf("Macaco_a %d passando...\n", i);
    pthread_mutex_unlock(&turno);
    pthread_mutex_unlock(&entrada);
    sleep(2);
    pthread_mutex_lock(&turno);
    qtd_a--;
    printf("Macaco_a %d passou caralhooo!! Ainda faltam %d\n", i, qtd_a);
    if (qtd_a == 0)
    {
      printf("Macaco_a %d acordando macacos B\n", i);
      pthread_cond_broadcast(&cond_b);
      printf("\n\n");
    }
    pthread_mutex_unlock(&turno);
    sleep(rand() % 10);
  }
}

void *lado_b(void *id)
{
  int i = *((int *)id);
  while (VERDADEIRO)
  {
    pthread_mutex_lock(&entrada);
    pthread_mutex_lock(&turno);
    while (qtd_a > 0 || gorila_esperando)
    {
      printf("Macaco_b %d na espera\n", i);
      pthread_cond_wait(&cond_b, &turno);
    }
    qtd_b++;
    printf("Macaco_b %d passando...\n", i);
    pthread_mutex_unlock(&turno);
    pthread_mutex_unlock(&entrada);
    sleep(2);
    pthread_mutex_lock(&turno);
    qtd_b--;
    printf("Macaco_b %d passou caralhooo!! Ainda faltam %d\n", i, qtd_b);
    if (qtd_b == 0)
    {
      printf("Macaco_b %d acordando macacos A\n", i);
      pthread_cond_broadcast(&cond_a);
      printf("\n\n");
    }
    pthread_mutex_unlock(&turno);
    sleep(rand() % 10);
  }
}

int main()
{
  pthread_t monkey_a[QTD_MACACOS_A], monkey_b[QTD_MACACOS_B], gorilas[QTD_GORILAS];
  int i, *cont;

  for (i = 0; i < QTD_GORILAS; i++)
  {
    cont = (int *)malloc(sizeof(int));
    *cont = i;
    pthread_create(&gorilas[i], NULL, &lado_gorila, (void *)cont);
  }

  for (i = 0; i < QTD_MACACOS_A; i++)
  {
    cont = (int *)malloc(sizeof(int));
    *cont = i;
    pthread_create(&monkey_a[i], NULL, &lado_a, (void *)cont);
  }

  for (i = 0; i < QTD_MACACOS_B; i++)
  {
    cont = (int *)malloc(sizeof(int));
    *cont = i;
    pthread_create(&monkey_b[i], NULL, &lado_b, (void *)cont);
  }

  for (i = 0; i < QTD_GORILAS; i++)
  {
    if (pthread_join(gorilas[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }
  for (i = 0; i < QTD_MACACOS_A; i++)
  {
    if (pthread_join(monkey_a[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }

  for (i = 0; i < QTD_MACACOS_B; i++)
  {
    if (pthread_join(monkey_b[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }

  return 0;
}