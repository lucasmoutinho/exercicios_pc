#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QTD_MACACOS_A 15
#define QTD_MACACOS_B 15
#define QTD_GORILAS 1
#define VERDADEIRO 1

pthread_mutex_t gorila_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t monkey_a_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t monkey_b_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t turno = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entrada = PTHREAD_MUTEX_INITIALIZER;

int qtd_a = 0;
int qtd_b = 0;
int qtd_g = 0;

void * lado_gorila(void * id){
    int i = *((int *) id);
    while(VERDADEIRO){
        printf("Gorila chegou\n");
        pthread_mutex_lock(&turno);
        pthread_mutex_lock(&entrada);
        qtd_g++;
        printf("Gorila %d passando...\n",i);
        sleep(5);
        qtd_g--;
        printf("Gorila %d passou!!!! Restam %d\n",i,qtd_g);
        pthread_mutex_unlock(&entrada);
        pthread_mutex_unlock(&turno);
        printf("\n\n");
        sleep(5);
    }
}

void * lado_a(void * id){
    int i = *((int *) id);
    while(VERDADEIRO){
        pthread_mutex_lock(&turno);
        pthread_mutex_lock(&monkey_a_lock);
        qtd_a++;
        if(qtd_a == 1){
            pthread_mutex_lock(&entrada);
        }
        printf("Macaco_a %d passando...\n",i);
        pthread_mutex_unlock(&monkey_a_lock);
        pthread_mutex_unlock(&turno);
        sleep(2);
        pthread_mutex_lock(&monkey_a_lock);
        qtd_a--;
        printf("Macaco_a %d passou caralhooo!! Ainda faltam %d\n",i,qtd_a);
        if(qtd_a == 0){
            pthread_mutex_unlock(&entrada);
            printf("\n\n");
        }
        pthread_mutex_unlock(&monkey_a_lock);
        sleep(rand()%10);
    }
}

void * lado_b(void * id){
    int i = *((int *) id);
    while(VERDADEIRO){
        pthread_mutex_lock(&turno);
        pthread_mutex_lock(&monkey_b_lock);
        qtd_b++;
        if(qtd_b == 1){
            pthread_mutex_lock(&entrada);
        }
        printf("Macaco_b %d passando...\n",i);
        pthread_mutex_unlock(&monkey_b_lock);
        pthread_mutex_unlock(&turno);
        sleep(2);
        pthread_mutex_lock(&monkey_b_lock);
        qtd_b--;
        printf("Macaco_b %d passou caralhooo!! Ainda faltam %d\n",i,qtd_b);
        if(qtd_b == 0){
            pthread_mutex_unlock(&entrada);
            printf("\n\n");
        }
        pthread_mutex_unlock(&monkey_b_lock);
        sleep(rand()%10);
    }
}

int main(){
    pthread_t monkey_a[QTD_MACACOS_A], monkey_b[QTD_MACACOS_B], gorilas[QTD_GORILAS];
    int i, *cont;

    for(i=0;i<QTD_GORILAS;i++){
        cont = (int*) malloc(sizeof(int));
        *cont = i;
        pthread_create(&gorilas[i],NULL,&lado_gorila,(void*)cont);
    }

    for(i=0;i<QTD_MACACOS_A;i++){
        cont = (int*) malloc(sizeof(int));
        *cont = i;
        pthread_create(&monkey_a[i],NULL,&lado_a,(void*)cont);
    }

    for(i=0;i<QTD_MACACOS_B;i++){
        cont = (int*) malloc(sizeof(int));
        *cont = i;
        pthread_create(&monkey_b[i],NULL,&lado_b,(void*)cont);
    }

    for(i = 0; i < QTD_GORILAS; i++){
        if(pthread_join(gorilas[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }
    for(i = 0; i < QTD_MACACOS_A; i++){
        if(pthread_join(monkey_a[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }

    for(i = 0; i < QTD_MACACOS_B; i++){
        if(pthread_join(monkey_b[i], NULL))
        {
            printf("Could not join thread %d\n", i);
            return -1;
        }
    }
   
   
    return 0;
}