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

static void * _abrirsupermercado();
static void * _camarasdeseguridad();
static void * _Guardia();


void productosregulados();
void cerrarSupermercado();
void initGuardia ();


void initGuardia()
{
	pthread_create(&guard,NULL,_Guardia,NULL);
	pthread_join(guard,NULL);
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
	getchar();
}

static void * _camarasdeseguridad()
{
	puts("\n-- Acceso al cuarto de grabación a Soporte Técnico --");
	sem_post(&tecnico);
	getchar();
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
	_abrirsupermercado();
	_camarasdeseguridad();
	productosregulados();
	cerrarSupermercado();	
}
#endif

/*void * _Guardia(void *arg)
{
	while(true)
	{
		sem_wait(&modulo[Guardia]);
		puts("-- Modulo Guardia --");
		if(open && close)
		{
			puts("Abriendo el Supermercado\nSupermercado Abierto");
			close = false;
		}
			if(regulados && !tickets && !close)	// Si es día de vender productos regulados porque hay productos regulados en almacén
			{
				if(!acceso)
				{
					// Antes de vender productos regulados se debe dar acceso al cuarto de grabación al módulo de Soporte Técnico
					puts("\n-- Acceso al cuarto de grabación a Soporte Técnico --");
					sem_post(&modulo[Tecnico]);	// Envío de senhal a Soporte Técnico
				}
				puts("Orden de vender productos Regulados");
				puts("\n-- El Guardia recibe tickets de productos regulados de pate del Supervisor --");
				// Cada día el supervisor cambia el número de tickets que entrega a los guardias de seguridad con un ramdom()
				// Esto se hace sobre la variable global tickets
					
				while(tickets<=288)	
				{
					//sem_wait(&modulo[Cliente]);							// Espera el cliente
						printf("\n-- Recibido ticket #%d", tickets+1); 	// El cliente recibió el ticket
						tickets++;										// Ahora hay un ticket menos
				}
				puts("\n Se agotaron los tickets. (Solo pasan los que tienen tickets)\n Si desean, pueden volver en otro momento");
				sem_post(&modulo[CajeraR]);	// Envía senhal a las cajeras para que paguen los de productos regulados
				
			}
			else if(!regulados && !close && !acceso)
			{
				puts("-- Aun no hay venta de productos regulados --");
				// Hoy no es día de productos regulados, así que antes de cerrar se debe dar acceso al cuarto de grabación al módulo de Soporte Técnico
				puts("\n-- Acceso al cuarto de grabación a Soporte Técnico al final del día --");
				sem_post(&modulo[Tecnico]);	// Envío de senhal a Soporte Técnico
			}
			else if(!close && !open)
			{
				puts("Cerrando el Supermercado\nSupermercado Cerrado");
				close = true;
			}
		puts("-----------------------\n");
	}
}
*/
