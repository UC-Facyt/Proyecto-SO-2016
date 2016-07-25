#ifndef UNIT_TESTING
#define UNIT_TESTING
#endif

#include "fake_estantes.h"
#include "fake_mercadeo.h"
#include "fake_supervisor.h"
#include "../../modulos/cajeras.h"

#include <stdio.h>
#include <stdlib.h>

static void* cliente_aleatorio(void *);

int main() {
    srand( time(NULL) );

    pthread_t clientes_aleatorios;

    printf("Inicializando modulo cajera\n");

    // Inicializando modulo
    init_cajeras(5, 5, 5);

    // encender_mitad_cajas();

    pthread_create( &clientes_aleatorios, NULL, cliente_aleatorio, NULL);

    // printf("Hola\n");
    join_cajeras();

    pthread_join(clientes_aleatorios, NULL);

    return 0;
}

static void* cliente_aleatorio(void *arg)
{
    int oneShot = 1;
    while (1)
    {
        // printf(" -------------- \n");
        switch (rand() % 20) {
        case 0:
            // printf("Agregado cliente normal\n");
            inc_cola_normal();
            break;
        case 1:
            // printf("Agregado cliente preferencial\n");
            inc_cola_pref();
            break;
        case 2:
            // printf("Agregado cliente regulado\n");
            inc_cola_regulados();
            break;
        default:
            break;
        }

        if ( oneShot ) {
            printf("One Shot\n");


            // cerrar_cajas();
            // abrir_cajas();
            apagar_mitad_cajas();

            oneShot = 0;
        }


    }
}
