/*
	Modulo realizado por:

	Ennio Gonzalez
	Christopher Silveiro
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


//Procedimiento para que la cajera se comunique
//con este modulo

void Comunicacion_Cajera_Estante(int numero, float cantidad);

//HILO REPONEDOR
static void * _Reponedor( void *args);

//ESTRUCTURA PARA LOS 9 ANAQUELES
float anaqueles[9]={10,10,10,10,10,10,10,10,10};

//HILOS PARA LOS 4 PROCESOS
pthread_t h,i,j,k;


void init_estantes(){
	int uno=1,dos=2,tres=3,cuatro=4;
	pthread_create(&h, NULL, _Reponedor, &uno);
	pthread_create(&i, NULL, _Reponedor, &dos);
	pthread_create(&j, NULL, _Reponedor, &tres);
	pthread_create(&k, NULL, _Reponedor, &cuatro);
}

void join_estantes(){
	pthread_join(h, NULL);
	pthread_join(j, NULL);
	pthread_join(j, NULL);
	pthread_join(k, NULL);
}


static void * _Reponedor( void *args){

	while(1)
	{
		if(*((int *) args)>=1 &&*((int *) args) <= 3){
			if(anaqueles[2*i-2]<5.000)
				anaqueles[2*i-2]=10;

			if(anaqueles[2*i-1]<5.000)
				anaqueles[2*i-1] =10;
		}else{

			if(anaqueles[2*i-2]<5.000)
				anaqueles[2*i-2]=10;

			if(anaqueles[2*i-1]<5.000)
				anaqueles[2*i-1] =10;

			if(anaqueles[2*i]<5.000)
				anaqueles[2*i] =10;
		}
	}
}


void Comunicacion_Cajera_Estante(int numero, float cantidad)
{
	anaqueles[i]-=cantidad;
}

#endif
