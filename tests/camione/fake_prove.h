#ifndef FAKEFAKE
#define FAKEFAKE

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include "../../modulos/camion.h"

pthread_t fake_prove;

void inittt();
void *fakifaki();
void clooose();


void postsemdesc();
void postenviado(int);
int trywaitcasa(int camionnn);

void initt()
{
	pthread_create(&fake_prove,NULL,fakifaki,NULL);
}

void *fakifaki()
{
	int i;
	while(1)
	{
		i = 0;
		while(!(trywaitcasa(i%6) + 1))
		{
			i++;
		}
		i = i % 6;
		postenviado(i);
		printf("Proveedor: enviado camion %d con productos\n",i);
		waitllego(i);
		postsemdesc(i);
		printf("Proveedor: Descargado camion %d\n",i);
	}
}

void clooose()
{
	pthread_join(fake_prove,NULL);
}


#endif