#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QNT_CACHORROS 100
#define QNT_GATOS 1
#define VERDADEIRO 1

pthread_mutex_t entrada = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gato_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cachorro_cond = PTHREAD_COND_INITIALIZER;

int gato_na_espreita=0;
int count_cachorro=0;


void *comer_gatos(void *arg){

  int i = *((int *)arg);

  while(1){
    printf("gato antes do loqueeeeeeeeeeeeeeeeee\n");
		pthread_mutex_lock(&turno);

		gato_na_espreita = 1;
    printf("\nGATO ESPERANDO OS CAES TERMINAREM\n\n");
		while( count_cachorro != 0 ){
			pthread_cond_wait(&gato_cond, &turno);
		}
		gato_na_espreita = 0;
    
		printf("Gato %d esta comendo\n", i);

		sleep(3);

		printf("Gato %d parou de se alimentar de produto alimenticio\n", i);

		pthread_cond_broadcast(&cachorro_cond);
		pthread_mutex_unlock(&turno);
		sleep(5);
	}
}

void *comer_cachorros(void *arg){

  int i = *((int *)arg);

  while(1){
		pthread_mutex_lock(&turno);


		while( gato_na_espreita ){
      pthread_cond_wait(&cachorro_cond, &turno);
		}
		count_cachorro ++;
		pthread_mutex_unlock(&turno);
		printf("Cachorro %d esta comendo\n", i);

		sleep(1);

		
		pthread_mutex_lock(&turno);
		count_cachorro --;
		printf("Cachorro %d parou de se alimentar de produto alimenticio -- faltam %d\n", i, count_cachorro);

		pthread_cond_signal(&gato_cond);
		pthread_mutex_unlock(&turno);
		sleep(20);
	}
}


int main(){
  pthread_t gato[QNT_GATOS], cachorro[QNT_CACHORROS];
  
  int  i, *id;

  for(i=0;i<QNT_GATOS;i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&gato[i], NULL, &comer_gatos, (void *)id);
  }
  for(i=0;i<QNT_CACHORROS;i++){
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&cachorro[i], NULL, &comer_cachorros, (void *)id);
  }
  
  for (i = 0; i < QNT_GATOS; i++){
    if (pthread_join(gato[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }
  for (i = 0; i < QNT_CACHORROS; i++){
    if (pthread_join(gato[i], NULL))
    {
      printf("Could not join thread %d\n", i);
      return -1;
    }
  }


  return 0;
}