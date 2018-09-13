/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 4 - PROGRAMAÇÃO CONCORRENTE - 2/2018
PROBLEMA DOS CANIBAIS - O COZINHEIRO TEM QUE PREPARAR 10 PORÇÕES DE COMIDA PROS CANIBAIS.
QUANDO O COZINHEIRO PREPARAR AS 10 PORÇÕES ELE PARA DE COZINHAR. OS CANIBAIS COMEÇAM A COMER
AS PORÇÕES, QUANDO ELAS CHEGAREM A 0 ELES DEVEM PARAR DE COMER. DEVEM EXISTIR VARIÁVEIS CONDICIONAIS
DE PARADA

OBS: UM WAIT SEM UM TESTE ANTES PROVAVELMENTE ESTÁ ERRADO

PSEUDOCÓDIGO:

INT COMIDA = 0;
PTHREAD_MUTEX L = PTHREAD...
PTHREAD_CONDT CA = PTHREAD_COND_INITIALIZER
PTHREAD_CONDT C0 = PTHREAD_COND_INITIALIZER

CANIBAIS C {1 .. N}{
  ...lock(l)
  while(comida == 0){
    pthread_cond wait(ca,l);
  }
  comida--
  if(comida == 0){
    pthread_cond signal(co);
  }
  unlock(l);
  //COMER
}

COZINHEIRO {1}{
  WHILE(TRUE){
    ...lock(l)
    while(comida!=0){
      pthread_cond WAIT(co,l);
    }
    //COZINHAR
    comida += X;
    pthread_cond BROADCAST(CA);
    ...unlock(l);
  }
};

*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define COZINHEIRO 1
#define CANIBAIS 20

pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER; /* lock pro contador*/
pthread_cond_t co = PTHREAD_COND_INITIALIZER; 
pthread_cond_t ca = PTHREAD_COND_INITIALIZER;
int comida = 0;

void *cozinhando(void *arg){
  int id = *((int *)arg);
  
  while(TRUE){
    pthread_mutex_lock(&l);
    while(comida != 0){
      printf("\nCozinheiro %d no aguardo, existem ainda %d porções de comida\n",id, comida);
      pthread_cond_wait(&co, &l);
    }

    printf("\nCanibais estão sem comida...\nCozinheiro %d indo preparar porções...\n", id);
    sleep(10);
    comida += 15;
    printf("Cozinheiro %d preparou %d porções de comida pros canibais laricados\n",id, comida);
    printf("\n");
    pthread_cond_broadcast(&ca);
    pthread_mutex_unlock(&l);
    sleep(1);
  }
  pthread_exit(0);
}

void *comendo(void *arg){
  int id = *((int*)arg);

  while (TRUE){
    pthread_mutex_lock(&l);
    while(comida == 0){
      printf("\nCanibal %d: estou aguardando o cozinheiro preparar porções\n", id);
      pthread_cond_wait(&ca, &l);
    }
    printf("Canibal %d: Vou comer... ainda existem %d porções\n", id, comida);
    sleep(1);
    comida--;
    if(comida == 0){
      printf("Canibal %d: Acabou a comida, vou acordar o cozinheiro\n", id);
      pthread_cond_signal(&co);
    }
    printf("\n");
    pthread_mutex_unlock(&l);
    sleep(1);
  }
  pthread_exit(0);
}

int main()
{

  pthread_t cozinheiro[COZINHEIRO], canibais[CANIBAIS];
  int i;
  int *id;

  for (i = 0; i < COZINHEIRO; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&cozinheiro[i], NULL, cozinhando, (void *)(id));
  }

  for (i = 0; i < CANIBAIS; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&canibais[i], NULL, comendo, (void *)(id));
  }

  pthread_join(cozinheiro[0], NULL);
  pthread_join(canibais[0], NULL);

  return 0;
}
