/*

ALUNO: LUCAS DA SILVA MOUTINHO - 15/0015747
PROFESSOR: ALCHIERI
ESTUDO DIRIGIDO 3 - PROGRAMAÇÃO CONCORRENTE - 2/2018
PROBLEMA DOS MACACOS - VÁRIOS MACACOS QUEREM ATRAVESSAR A CORDA, MACACOS DA ILHA A QUEREM IR PRA B E VICE-VERSA.
NÃO PODEM HAVER MACACOS ANDANDO EM DIREÇÕES OPOSTAS, PORÉM, VÁRIOS MACACOS DA MESMA ILHA PODEM ATRAVESSAR AO MESMO TEMPO.
MACACOS A QUEREM IR AO LESTE EM DIREÇÃO A ILHA B
MACACOS B QUEREM IR AO OESTE EM DIREÇÃO A ILHA A

*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define TRUE 1
#define MACACOS_A 5
#define MACACOS_B 5

pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER; /* lock pro contador*/
pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER; /* lock pra que macacos A não atravessem a corda*/
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER; /* lock pra que macacos B não atravessem a corda*/
int a = 0, b = 0; /* numero de macacos atravessando a corda*/

void *travessia_leste(void *arg){
	int id = *((int *) arg);

	while(TRUE){
		pthread_mutex_lock(&turno);
		pthread_mutex_lock(&lock_a);
		a++;
		if(a == 1){
			pthread_mutex_lock(&lock_b);
		}
		pthread_mutex_unlock(&lock_a);
		pthread_mutex_unlock(&turno);

		printf("MACACÃO A:%d -- SAINDO DA ILHA A NA DIREÇÃO LESTE\n", id);
		sleep(2);

		pthread_mutex_lock(&lock_a);
		a--;
		printf("MACACÃO A:%d -- CHEGOU NA ILHA B PELA DIREÇÃO LESTE -- RESTANTES:%d\n", id, a);
		if(a == 0){
			pthread_mutex_unlock(&lock_b);
			printf("\n\n");
		}
		pthread_mutex_unlock(&lock_a);
		sleep(1);
	}

	pthread_exit(0);	

}

void *travessia_oeste(void* arg){
	int id = *((int *) arg);

	while(TRUE){
		pthread_mutex_lock(&turno);
		pthread_mutex_lock(&lock_b);
		b++;
		if(b == 1){
			pthread_mutex_lock(&lock_a);
		}
		pthread_mutex_unlock(&lock_b);
		pthread_mutex_unlock(&turno);

		printf("MACACÃO B:%d -- SAINDO DA ILHA B NA DIREÇÃO OESTE\n", id);
		sleep(2);

		pthread_mutex_lock(&lock_b);
		b--;
		printf("MACACÃO B:%d -- CHEGOU NA ILHA A PELA DIREÇÃO OESTE -- RESTANTES:%d\n", id, b);
		if(b == 0){
			pthread_mutex_unlock(&lock_a);
			printf("\n\n");
		}
		pthread_mutex_unlock(&lock_b);
		sleep(1);
	}

	pthread_exit(0);

}

int main(){

	pthread_t a[MACACOS_A], b[MACACOS_B];
	int i;
    int *id;

    for (i = 0; i < MACACOS_A ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&a[i], NULL, travessia_leste, (void *) (id));
    }

    for (i = 0; i < MACACOS_B ; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;
        pthread_create(&b[i], NULL, travessia_oeste, (void *) (id));
    }

    pthread_join(a[0],NULL);
    pthread_join(b[0],NULL);

    return 0;

}