#ifndef CAMIONITO_FASHION
#define CAMIONITO_FASHION

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>


typedef struct
{
	int categoria;
	int peso; 
	int regulado;
}producto;

typedef struct 
{
	producto rubros[30];
	int num;
	sem_t enviado,casa,llego,descarga;
}camion; 

pthread_t camionito[5];
sem_t descarga;

int v[5],peso[5];

camion camiones[5];

void *camionthread(void *arg);
void closecamion();
void inicamion();
void postsemdesc(int camionnn);
void postenviado(int);
int trywaitcasa(int camionnn);
void waitllego(int camionnn);

void inicamion()
{
	int i;

	srand(time(NULL));
	for(i = 0;i<5;i++)
	{
		v[i] = i;
		sem_init(&camiones[i].enviado,1,0);
		sem_init(&camiones[i].casa,1,1);
		sem_init(&camiones[i].llego,1,0);
		sem_init(&camiones[i].descarga,1,0);
		pthread_create(&camionito[i],NULL,camionthread,(void *)&v[i]);
	}
}

void *camionthread(void *arg)
{
	int camionNo = *((int *)arg);
	
	peso[camionNo] = (rand()%2) ? 100 : 150;

	while(1)
	{
		sem_wait(&camiones[camionNo].enviado);
		printf("El camion %d acaba de sarpar\n",camionNo);

		printf("El camion %d llego\n",camionNo);
		sem_post(&camiones[camionNo].llego);
		sem_wait(&camiones[camionNo].descarga);
		printf("El camion %d fue descargado\n",camionNo);
		printf("El camion %d regresa a proveer\n",camionNo);
		sem_post(&camiones[camionNo].casa);
	}
}

void waitllego(int camionnn)
{
	sem_wait(&camiones[camionnn].llego);
}

void postsemdesc(int camionnn)
{
	sem_post(&camiones[camionnn].descarga);
}

void postenviado(int camionnn)
{
	sem_post(&camiones[camionnn].enviado);
}

int trywaitcasa(int camionnn)
{
	return sem_trywait(&camiones[camionnn].casa);
}

void closecamion()
{
	int i;
	for(i=0;i<5;i++)
	{
		pthread_join(camionito[i],NULL);
	}
}

#endif