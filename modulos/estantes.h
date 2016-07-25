/*
	Modulo realizado por:

	Ennio Gonzalez
	Christopher Siveiro
	Juan Sanchez
*/

#ifndef ESTANTES_H
#define ESTANTES_H


#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>


void init_estantes();
void join_estantes();

//Funcion para que la cajera se comunique
//con este modulo

int Comunicacion_Cajera_Estante(int numero, float cantidad);

//Procedimiento para Activar/Deshabilitar los Reponedores
void Activar_Reponedores(int est);

//HILO REPONEDOR
static void * _Reponedor( void *args);

//ESTRUCTURA PARA LOS 9 ANAQUELES
static float anaqueles[9]={10,10,10,10,10,10,10,10,10};

//HILOS PARA LOS 4 PROCESOS
static pthread_t t1, t2, t3, t4;

//Variable de estado para activar o desactivar todos los estantes
static int Estado=0;

static sem_t estado[4], permiso_anaqueles[9];

static int uno=1, dos=2,tres=3,cuatro=4;

void init_estantes()
{
	int i;
	for ( i = 0; i < 4; i++ ) {
		sem_init( &estado[i], 0, 1 );
	}

	for ( i = 0; i < 9; i++ ) {
		sem_init( &permiso_anaqueles[i], 0, 1 );
	}

	pthread_create(&t1, NULL, _Reponedor, (void *)&uno);
	pthread_create(&t2, NULL, _Reponedor, (void *)&dos);
	pthread_create(&t3, NULL, _Reponedor, (void *)&tres);
	pthread_create(&t4, NULL, _Reponedor, (void *)&cuatro);
}

void join_estantes()
{
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
}


static void * _Reponedor( void *args){
	int id = *((int *) args);

	while(1)
	{
		sem_wait( &estado[id-1] );
			printf("Swagg");
			if (id>=1 && id <= 3) {
				if(anaqueles[2*id-2]<5.000)
					anaqueles[2*id-2]=10;

				if(anaqueles[2*id-1]<5.000)
					anaqueles[2*id-1] =10;
			} else {
				if(anaqueles[2*id-2]<5.000)
					anaqueles[2*id-2]=10;

				if(anaqueles[2*id-1]<5.000)
					anaqueles[2*id-1] =10;

				if(anaqueles[2*id]<5.000)
					anaqueles[2*id] =10;
			}
		sem_post( &estado[id-1] );
	}
}


int vender(int numero, float cantidad)
{
	sem_wait( &permiso_anaqueles[numero] );
		int res = 0;
		if (anaqueles[numero] >= cantidad) {
			anaqueles[numero] -= cantidad;
			res = 1;
		}
		return res;
	sem_post( &permiso_anaqueles[numero] );
}


void activar_estantes()
{
	int i;
	for (i = 0; i < 4; i++) {
		sem_post(&estado[i]);
	}
}

void desactivar_estantes()
{
	int i;
	for (i = 0; i < 4; i++) {
		sem_wait(&estado[i]);
	}
}

#endif
