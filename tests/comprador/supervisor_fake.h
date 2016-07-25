#ifndef FAKE_GPS
#define FAKE_GPS

#include <stdio.h>
#include <semaphore.h>
#include <stdlib>
#include <pthread.h>

sem_t pedido;

pthread_t super;

void intiillimani();
void waiteoensupe();

void intiillimani()
{
	srand(time(NULL));
	sem_init(&pedido,0,0);
	pthread_create(&super,NULL,super,NULL;
}

void *super()
{
	while(1)
	{
		printf("Soy Diosdado Cabeyo\n")

		if(rand() < 5000)
		{
			sem_post(&pedido);
		}
	}
}

void waiteoensupe()
{
	sem_wait(&pedido);
}

#endif