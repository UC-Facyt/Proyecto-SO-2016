#ifndef CLIENTE_H
#define CLIENTE_H

#define cantidad_clientes 10

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "cajeras.h"


static void *cliente(void *data);
static pthread_t hilos_clientes[cantidad_clientes];


void init_cliente()
{
    int i;
    for (i = 0; i < cantidad_clientes; i++) {
        pthread_create( &hilos_clientes[i], NULL, cliente, NULL );
    }
}

void join_cliente()
{
    int i;
    for (i = 0; i < cantidad_clientes; i++) {
        pthread_join( hilos_clientes[i], NULL );
    }
}

static void *cliente(void *data) {
    while(1)
    {
        switch(rand() % 3) {
        case 0:
            inc_cola_normal();
        case 1:
            inc_cola_pref();
        case 2:
            inc_cola_regulados();
        }
    }
}

#endif
