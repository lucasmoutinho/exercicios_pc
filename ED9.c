/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 9 - PROGRAMAÇÃO CONCORRENTE - 2/2018
PROBLEMA DO BARBEIRO - EXISTEM X CADEIRAS DE ESPERA NUMA BARBEARIA. QUANDO N TEM NINGUEM, O BARBEIRO DORME. SE AS CADEIRAS ESTIVEREM
CHEIAS, OS CLIENTES VÃO EMBORA. CADA UM DOS CLIENTES ESPERA NUMA CADEIRA ATE CONSEGUIR IR PRA CADEIRA DO BARBEIRO.
SE AS CADEIRAS ELES VAO EMBORA, POR ISSO TEM QUE USAR O TRY_WAIT AO INVES DO WAIT, N SE QUER FILA.

SEMAFORO PRA CONTAR AS CADEIRAS, OUTRO PRA CADEIRA DO BARBEIRO, DOIS PRA SINCRONIZAR O CLIENTE SENTAR E ACORDAR O BARBEIRO, E OUTRO PRA QND O BARBEIRO PARAR O CLIENTE VAZAR

SOLUÇÃO:

sem_t cadeiras = x;
sem_t cad_b = 1;
sem_t barbeiro = 0
sem_t cliente = 0;

barbeiro(){
  while(TRUE){
    sem_wait(barbeiro);
    // cortar cabelo
    sem_post(cliente);
  }
}

cliente(){
  while(TRUE){
    if(semtrywait(cadeiras) == 0){
      //entrou
      sem_wait(cad_b)
      sem_post(cadeiras)
      sem_post(barbeiro)
      sem_wait(cliente)
      sem_post(cad_b)
    }
    else{
      //nao entrou
    }
  }
}

*/

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

void *barbeiros(void *arg){

  int id = *((int *)arg);

  while (TRUE){
    sem_wait(&barb);
    sleep(3);
    printf("Barbeiro %d acabou de cortar o cabelo, foi dormir\n", id);
    sem_post(&cortando);
  }

  pthread_exit(0);
}

void *clientes(void *arg){

  int id = *((int *)arg);

  while (TRUE){

    if(sem_trywait(&cadeira) == 0){
      printf("Cliente %d entrou na loja e sentou numa cadeira\n", id);
      sem_wait(&cadeira_barbeiro);
      printf("Cliente %d se levantou para ir até a cadeira do barbeiro, acordando o barbeiro\n", id);
      sem_post(&cadeira);
      sem_post(&barb);
      printf("Cliente %d está cortando o cabelo\n", id);
      sem_wait(&cortando);
      printf("Cliente %d teve o cabelo cortado, vazando da loja\n", id);
      sem_post(&cadeira_barbeiro);
    }
    else{
      printf("Cliente %d não entrou na loja\n", id);
    }
    sleep(15);
  }

  pthread_exit(0);
}

int main(){

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