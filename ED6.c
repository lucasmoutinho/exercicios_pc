/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 6 - PROGRAMAÇÃO CONCORRENTE - 2/2018

O ESTACIONAMENTO DE UMA UNIVERSIDADE POSSUI 30 VAGAS. ENQUANTO O MESMO POSSUI VAGAS, NÃO EXISTE
PRIORIDADE PRA QUEM ESTACIONA. QUANDO O MESMO ESTA CHEIO, FORMA-SE UMA "FILA" (IMAGINARIA)
PROFESSORES TEM PRIORIDADE ACIMA DOS FUNCIONARIOS E ESTES ACIMA DOS ALUNOS

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QTD_PROFESSORES 3
#define QTD_FUNCIONARIOS 4
#define QTD_ALUNOS 5
#define QTD_ESTACIONAMENTO 5
#define TRUE 1

pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cp = PTHREAD_COND_INITIALIZER;
pthread_cond_t cf = PTHREAD_COND_INITIALIZER;
pthread_cond_t ca = PTHREAD_COND_INITIALIZER;

int vagas = 0;
int professores_esperando = 0;
int funcionarios_esperando = 0;

void *vaga_professor(void *id)
{
  int i = *((int *)id);

  
  while(TRUE){
    pthread_mutex_lock(&turno);
    professores_esperando++;
    while(vagas == QTD_ESTACIONAMENTO){
      printf("Estacionamento lotado -- Professor %d na fila do estacionamento...\n", i);
      pthread_cond_wait(&cp, &turno);
    }
    professores_esperando--;

    vagas++;
    printf("Professor %d entrando no estacionamento -- Vagas restantes: %d\n", i, QTD_ESTACIONAMENTO - vagas);
    pthread_mutex_unlock(&turno);

    sleep(10);

    pthread_mutex_lock(&turno);
    vagas--;
    if (vagas == QTD_ESTACIONAMENTO - 1){
      printf("Liberado vagas, acordando geral...\n");
      pthread_cond_signal(&cp);
      pthread_cond_signal(&cf);
      pthread_cond_signal(&ca);
    }
    printf("Professor %d saindo do estacionamento -- Vagas restantes: %d\n", i, QTD_ESTACIONAMENTO - vagas);
    pthread_mutex_unlock(&turno);
    sleep(2);
  }
  pthread_exit(0);
}

void *vaga_funcionarios(void *id){
  int i = *((int *)id);


  while(TRUE){

    pthread_mutex_lock(&turno);
    funcionarios_esperando++;
    while (vagas == QTD_ESTACIONAMENTO || professores_esperando > 0){
      printf("Estacionamento lotado -- Funcionario %d na fila do estacionamento...\n", i);
      pthread_cond_wait(&cf, &turno);
    }
    funcionarios_esperando--;

    vagas++;
    printf("Funcionario %d entrando do estacionamento -- Vagas restantes: %d\n", i, QTD_ESTACIONAMENTO - vagas);
    pthread_mutex_unlock(&turno);

    sleep(8);

    pthread_mutex_lock(&turno);
    vagas--;
    if (vagas == QTD_ESTACIONAMENTO - 1){
      printf("Liberado vagas, acordando geral...\n");
      pthread_cond_signal(&cp);
      pthread_cond_signal(&cf);
      pthread_cond_signal(&ca);
    }
    printf("Funcionario %d saindo no estacionamento -- Vagas restantes: %d\n", i, QTD_ESTACIONAMENTO - vagas);
    pthread_mutex_unlock(&turno);
    sleep(2);
  }
  pthread_exit(0);
}

void *vaga_alunos(void *id)
{
  int i = *((int *)id);

  while (TRUE)
  {
    pthread_mutex_lock(&turno);
    while (vagas == QTD_ESTACIONAMENTO || professores_esperando > 0 || funcionarios_esperando > 0)
    {
      printf("Estacionamento lotado -- Aluno %d na fila do estacionamento...\n", i);
      pthread_cond_wait(&ca, &turno);
    }

    vagas++;
    printf("Aluno %d entrando do estacionamento -- Vagas restantes: %d\n", i, QTD_ESTACIONAMENTO - vagas);
    pthread_mutex_unlock(&turno);

    sleep(5);

    pthread_mutex_lock(&turno);
    vagas--;
    if(vagas == QTD_ESTACIONAMENTO - 1){
      printf("Liberado vagas, acordando geral...\n");
      pthread_cond_signal(&cp);
      pthread_cond_signal(&cf);
      pthread_cond_signal(&ca);
    }
    printf("Aluno %d saindo no estacionamento -- Vagas restantes: %d\n", i, QTD_ESTACIONAMENTO - vagas);
    pthread_mutex_unlock(&turno);
    sleep(2);
  }

  pthread_exit(0);
}

int main()
{
  pthread_t professores[QTD_PROFESSORES], funcionarios[QTD_FUNCIONARIOS], alunos[QTD_ALUNOS];
  int i, *cont;

  for (i = 0; i < QTD_PROFESSORES; i++)
  {
    cont = (int *)malloc(sizeof(int));
    *cont = i;
    pthread_create(&professores[i], NULL, &vaga_professor, (void *)cont);
  }

  for (i = 0; i < QTD_FUNCIONARIOS; i++)
  {
    cont = (int *)malloc(sizeof(int));
    *cont = i;
    pthread_create(&funcionarios[i], NULL, &vaga_funcionarios, (void *)cont);
  }

  for (i = 0; i < QTD_ALUNOS; i++)
  {
    cont = (int *)malloc(sizeof(int));
    *cont = i;
    pthread_create(&alunos[i], NULL, &vaga_professor, (void *)cont);
  }

  for (i = 0; i < QTD_PROFESSORES; i++)
  {
    if (pthread_join(professores[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }
  for (i = 0; i < QTD_FUNCIONARIOS; i++)
  {
    if (pthread_join(funcionarios[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }

  for (i = 0; i < QTD_ALUNOS; i++)
  {
    if (pthread_join(alunos[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }

  return 0;
}