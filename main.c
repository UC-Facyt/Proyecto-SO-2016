#include <stdio.h>

#include "modulos/cajeras.h"
#include "modulos/servicio_tecnico.h"
#include "fakies/fake_prove.h"
#include "modulos/camion.h"
#include "modulos/cliente.h"

int main()
{
	init_cajeras(5, 5, 5);


	init_TI();
	init_cliente();
	inicamion();
	initt();

	closecamion();
	clooose();
	join_TI();
	join_cliente();
	join_cajeras();

	return 0;
}
