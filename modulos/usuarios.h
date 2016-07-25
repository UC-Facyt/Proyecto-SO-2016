/*
Carlos Escalona
Gibran Guerra
Gilbert Chavez 
Eduardo Rodriguez
*/
//Modulo de Usuario/Cliente

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

void *tarea_usuario();
void *preferencial();
int con=1;
int producto=10;
sem_t clientes_pref, clientes_reg, clientes_normal;

void inic_usuario()
{
	pthread_t usuario;
	pthread_t edad;
	int erroru;
	int error3;

	while(1)
	{
		//funcion generadora de desastres
		if ((con % 3) == 0)
		{
			if ((con % 2)== 0)
			{
				//residuos solidos 
			}
			else 
			{	
				if ((con % 5) == 0)
				{
				//residuos liquidos 
				}
			}
		}
		erroru = pthread_create (&usuario, NULL, tarea_usuario, NULL);
		error3 = pthread_create (&edad, NULL, preferencial, NULL);
	}

	
}


void *tarea_usuario()
{
	if(producto==regulado)
	{
		signal(clientes_reg);
	}else
	{
		signal(clientes_normal);
	}
}

void *preferencial()
{
	if(producto==regulado)
	{
		signal(clientes_reg);
	}else
	{
		signal(clientes_pref);
	}
}


