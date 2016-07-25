#ifndef _GUARDIA_H_
#define _GUARDIA_H_
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define true 1
#define false 0
#define MAX_REGULADO 288


sem_t guardia, regulado, venta,  tecnico;
pthread_t guard;
volatile int open = false;
volatile int close = true;
volatile int ordenRegulado = true;

static void  _abrirsupermercado();
static void  _camarasdeseguridad();
static void * _Guardia();


void productosregulados();
void cerrarSupermercado();
void initGuardia ();

void cerrasion();

void _postGuardia();


void _postGuardia();
{
	sem_post(&guardia);
}

void cerrasion()
{
	pthread_join(guard,NULL);
}

void initGuardia()
{
	sem_init(&guardia,0,0);
	sem_init(&regulado,0,0);
	sem_init(&venta,0,0);
	sem_init(&tecnico,0,0);
	pthread_create(&guard,NULL,_Guardia,NULL);
}

static void * _abrirsupermercado()//guardia
{
	while (true)
	{
		sem_wait(&guardia);
		puts("--Modulo Guardia");
		if (open && !close)
			puts ("--Supermercado abierto--");
		else
			puts ("--Supermercado aun cerrado--");
	}
}

static void * _camarasdeseguridad()
{
	puts("\n-- Acceso al cuarto de grabación a Soporte Técnico --");
	sem_post(&tecnico);
}

//procedimientos de guardias
void productosregulados()
{
	int i;
	if (ordenRegulado) //variable de supervisor
	{
		puts("Orden de vender productos Regulados");
		puts("\n-- El Guardia recibe tickets de productos regulados de pate del Supervisor --");
		
		for (i=0; i<MAX_REGULADO; i++)
			printf("\n-- Recibido ticket #%d", i);
		puts("\n Se agotaron los tickets. (Solo pasan los que tienen tickets)\n Si desean, pueden volver en otro momento");
		//sem_post(&cajeras);	/*Modulo cajeras*/		
	}
	else
		puts("\nHoy no es día de productos regulados");
	
	getchar();
}



void cerrarSupermercado()
{
	if (!open && !close){ //variables de supervisor
		puts("Cerrando el Supermercado\nSupermercado Cerrado");
		close =true;
	}
}

static void * _Guardia()
{
	while(1)
	{
		sem_wait(&guardia);
		sem_post(&guardia);

		
	}
}
#endif