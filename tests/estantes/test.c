#ifndef UNIT_TESTING
#define UNIT_TESTING
#endif

#include "../../modulos/estantes.h"

#include <stdio.h>
#include <stdlib.h>


int main() {
    srand( time(NULL) );

    pthread_t prueba;

    printf("Inicializando modulo estantes \n");

    init_estantes();

    desactivar_estantes();
    sleep(2);
    printf("\nLISTOLIN\n");
    activar_estantes();
    sleep(2);
    printf("\nLISTOLIN\n");
    desactivar_estantes();

    join_estantes();

    return 0;
}
