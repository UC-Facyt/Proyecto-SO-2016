#include "servicio_tecnico.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

typedef struct{
	sem_t fallo_de_energia;
} Shared;

void* servicioTecnico(void *);

int main() //Loop principal
{
	pthread_t IT;
	Shared data;
	sem_init(&data.fallo_de_energia, 1, 0);
	printf("Hello world!\n");
	srand(time(NULL));
	pthread_create(&IT, NULL, servicioTecnico, (void *)&data);

	while(1){
		if(rand() < RAND_MAX * 0.1){
			sem_post(&data.fallo_de_energia);
	    	}
	    	sleep(1);
			sem_wait( &func[1] );
    	}
    sleep(5);
    return 0;
}

void* servicioTecnico(void * data){
    //variables compartidas
    Shared *shared = (Shared*) (data);
	while(1){
		sem_wait(&shared->fallo_de_energia);
        	printf("Hubo un fallo de energia!\n");
	}
}
