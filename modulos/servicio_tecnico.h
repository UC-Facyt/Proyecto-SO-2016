/* Modulo hecho por:
 * Mura Javier		21.456.202
 * Labbe Andres		22.728.231
 * Capriles Oswaldo	26.026.458
 * Victor Tortolero	24.569.609
 */

#ifndef SERVICIO_TECNICO_H
#define SERVICIO_TECNICO_H


#include "cajeras.h"


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


/* Estas funciones seran usadas para generar reportes!
 * Pasar un struct con los datos necesarios
 */
void encender_servicio_tecnico();
void apagar_servicio_tecnico();
void reporteDiario(void *);
void reporteSemanal(void *);

/* Funciones de nuestro modulo */
void init_TI();
void join_TI();

/* Funciones locales de nuestro modulo */
static void * servicioTecnico(void *);
static int    nosVolvimosLocos();
static int    llamarACorpoelec();

void apagar_mitad_cajas();
void encender_mitad_cajas();

// Variable para controlar si el modulo esta encendido o apagado.
static sem_t estado;

pthread_t TI;
static sem_t reporteD, reporteS;
static int luz = 1;


void init_TI()
{
	srand(time(NULL));
	pthread_create(&TI, NULL, servicioTecnico, NULL);
	sem_init(&reporteD, 0, 1);
	sem_init(&reporteS, 0, 1);
	sem_init(&estado, 0, 1);
}

void join_TI()
{
	pthread_join(TI, NULL);
}

static void * servicioTecnico(void *data)
{
	while(1)
	{
		/*
			se hace un wait a estado para verificar si nuestro modulo
			esta encendido, el supervisor podra llamar a la funcion
			apagar_servicio_tecnico() para apagar el modulo
			cuando se cierre el mercado
		*/
		sem_wait(&estado);
			/* Si no hay luz.. */
			if (!luz)
			{
				if (llamarACorpoelec())
				{
					printf("Llego la luz!\n");
					encender_mitad_cajas(); // Esta funcion es del modulo de cajeras
				}
			}
			else
			{
				if (!nosVolvimosLocos())
				{
					printf("Hubo un fallo de energia! Se prendio la plantas\n");
					apagar_mitad_cajas(); // Esta funcion es del modulo de cajeras
				}
			}
		sem_post(&estado);
	}
}

/* Simula si se va la luz */
static int nosVolvimosLocos()
{
	if (rand() < RAND_MAX * 0.05)
		luz = 0;
	return luz;
}

/* Simula que llegue la luz antes de irse */
static int llamarACorpoelec()
{
	if (rand() < RAND_MAX * 0.8)
		luz = 1;
	return luz;
}

void reporteDiario(void *reporte)
{
	// Hacerle casting a reporte e inicializarla
	sem_wait(&reporteD);
		printf("Generando reporte diario");
		// Imprimir data del reporte o guardarla
	sem_post(&reporteD);
}

void reporteSemanal(void *reporte)
{
	// Hacerle casting a reporte e inicializarla
	sem_wait(&reporteS);
		printf("Generando reporte semanal");
		// Imprimir data del reporte o guardarla
	sem_post(&reporteS);
}

void apagar_servicio_tecnico()
{
	sem_wait(&estado);
}

void encender_servicio_tecnico()
{
	sem_post(&estado);
}

#endif
