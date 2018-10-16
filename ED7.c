/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 7 - PROGRAMAÇÃO CONCORRENTE - 2/2018
PROBLEMA DOS PRODUTORES E CONSUMIDORES - O PRODUTOR TEM UM BUFFER LIMITADO DE TAMANHO N ONDE PRODUZ DADOS ( NO CASO, INTEIROS ), 
QUANDO ELE ENCHE O BUFFER ELE DA UM SLEEP E ACORDA O CONSUMIDOR. QUANDO O BUFFER TA VAZIO, O CONSUMIDOR DA UM SLEEP E ACORDA O PRODUTOR
O TEMPO PRA PRODUZIR E CONSUMIR UM INTEIRO PODE SER UM SLEEP
SOLUÇÃO FEITA UTILIZANDO SEMÁFOROS

*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define TRUE 1
#define PRODUTOR 10
#define CONSUMIDOR 10
#define TAMANHO 5

pthread_t produtor[PRODUTOR], consumidor[CONSUMIDOR];
pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;
sem_t posicoes_livres;
sem_t posicoes_ocupadas;

int count = 0;
int item = 0;
int buffer[TAMANHO];

void inicializaBuffer(){
  int i = 0;
  for (i = 0; i < TAMANHO; i++)
  {
    buffer[i] = -1;
  }
}

void showBuffer(){
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

  while(TRUE){
    sem_wait(&posicoes_livres);
    pthread_mutex_lock(&l);
    item = (rand() % 10);
    printf("Produtor %d: Estou produzindo o item (%d) para o buffer\n", id, item);
    sleep(1);
    buffer[count] = item;
    count++;
    showBuffer();
    printf("\n");
    pthread_mutex_unlock(&l);
    sem_post(&posicoes_ocupadas);
    sleep(rand() % 10);
  }

  pthread_exit(0);
}

void *consumindo(void *arg)
{
  int id = *((int *)arg);

  while (TRUE)
  {
    sem_wait(&posicoes_ocupadas);
    pthread_mutex_lock(&l);
    count--;
    printf("Consumidor %d: Estou consumindo o item (%d) do buffer\n", id, buffer[count]);
    sleep(1);
    buffer[count] = -1;
    showBuffer();
    printf("\n");
    pthread_mutex_unlock(&l);
    sem_post(&posicoes_livres);
    sleep(rand() % 10);
  }

  pthread_exit(0);
}


int main(){

  int i;
  int *id;

  sem_init(&posicoes_livres, 0, TAMANHO);
  sem_init(&posicoes_ocupadas, 0, 0);

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