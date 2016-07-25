#include <stdio.h>
#include "modulos/cajeras.h"
#include "modulos/servicio_tecnico.h"
#include "modulos/camion.h"

int main()
{
	init_cajeras(5.5,5.5,5.5);
	init_TI();
	inicamion();


	closecamion();
	join_TI();
	join_cajeras();
	return 0;
}