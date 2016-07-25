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
void * _Reponedor( void *args);

//ESTRUCTURA PARA LOS 9 ANAQUELES
float anaqueles[9]={10,10,10,10,10,10,10,10,10};

//HILOS PARA LOS 4 PROCESOS
pthread_t h,i,j,k;

//Variable de estado para activar o desactivar todos los estantes
int Estado=0;

int uno=1,dos=2,tres=3,cuatro=4;
void init_estantes(){
	
	pthread_create(&h, NULL, _Reponedor,(void *)&uno);
	pthread_create(&i, NULL, _Reponedor,(void *)&dos);
	pthread_create(&j, NULL, _Reponedor,(void *)&tres);
	pthread_create(&k, NULL, _Reponedor,(void *)&cuatro);
}

void join_estantes(){
	pthread_join(h, NULL);
	pthread_join(j, NULL);
	pthread_join(j, NULL);
	pthread_join(k, NULL);
}


void * _Reponedor( void *args){

	while(1)
	{
		while(!Estado);
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


int Comunicacion_Cajera_Estante(int numero, float cantidad)
{
	if(anaqueles[numero-1]>=cantidad)
		anaqueles[numero-1]-=cantidad;

	return anaqueles[numero-1]>=cantidad;
}


void Activar_Reponedores(int est){
	Estado=est;
}

#endif
