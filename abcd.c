
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define M 5
#define N 4
#define TOTAL N+M

pthread_barrier_t barrier;

void imprime (char * string) {
  printf("%s", string);
}

void *processo_AC (void *args) {
  imprime("A");
  pthread_barrier_wait(&barrier);
  pthread_barrier_wait(&barrier);
  imprime("C");
  pthread_barrier_wait(&barrier);
  pthread_exit(0);
}

void * processo_BD (void * args) {
  pthread_barrier_wait(&barrier);
  imprime("B");
  pthread_barrier_wait(&barrier);
  pthread_barrier_wait(&barrier);
  imprime("D");
  pthread_exit(0);
}

int main(){
	int i;
	pthread_t m[M];
	pthread_t n[N];
  pthread_barrier_init(&barrier, NULL, TOTAL);
  int *id;


	for (i = 0; i < N; i++)	{
		id = (int *)malloc(sizeof(int));
		*id = i;
		pthread_create(&n[i], NULL, processo_AC, (void *)(id));
	}

  for (i = 0; i < M; i++) {
    id = (int *)malloc(sizeof(int));
    *id = i;
    pthread_create(&m[i], NULL, processo_BD, (void *)(id));
  }

  for (i = 0; i < N; i++) pthread_join(n[i], NULL);
  for (i = 0; i < N; i++) pthread_join(m[i], NULL);
	return 0;
}
