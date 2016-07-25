#ifndef UNIT_TESTING
#define UNIT_TESTING
#endif

#include "fake_cajeras.h"
#include "../../modulos/proveedor.h"

#include <stdio.h>
#include <stdlib.h>


int main() {
    printf("Inicializando modulos\n");

    // Inicializando modulo
    init_TI();

    join_TI();

    return 0;
}
