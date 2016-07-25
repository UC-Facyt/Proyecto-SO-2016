/** Modulo: Proveedor
	* Wilkel Giovanni
	* Freddy Rodriguez
	* Geraldine Herrera
	* Jordan Herrera
	* */

#ifndef _PROVEEDOR_H_
#define _PROVEEDOR_H_

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define N 9
#define M 5
#define true 1
#define false 0
#define MAX_RUBROS 650

// Pedido de Rubro
typedef struct
{
	float pedido; // Almacena el Pedido
	float vendido; // Descontador de lo vendido
}pedidoR;

// Categoría, 0 Hortaliza,1 Bebida,2 Charcutería,3 Carnicería,4 Pescadería,5 Enlatado,6 Producto de Limpieza,7 Producto de Aseo Personal,8 Perfume.
typedef pedidoR categoria[N]; // Cada campo indica la cantidad en kg/unidad del rubro.

pthread_t prov;
// Semáforos
sem_t sprov;
sem_t camionEnviado;
sem_t Camion[M-1];

// Globales
categoria rubros; // Rubros actuales en el supermercado (pedido = total que se adquirió, vendido = total actual)
categoria list; // Lista de Rubros a adquirir por el Comprador al Proveedor. Una vez adquirida se anexa a Rubros

// Compartidas
volatile int camiones = false;
volatile int aceptado = false;
volatile int acceso = false;

// Prototipos de Supervisor
int _hizoPedido();
void _cerrarPedido();

// Prototipos de Comprador
void _postConExito();
void _waitConExito();
void _postValidezPedido();
void _waitPedidoRecibido();
void _postComprador();

// Prototipos Própios
void _prepararCamion();
void _listaRechazada();
void _llenarCamiones();
int _estadoCamion(int c);

int estado[M];
float VecPeso[M] = {100, 100, 150, 150, 150};
float peso,peso_restante;

void _initProveedor()
{
	pthread_create(&prov,NULL,_Proveedor,NULL);
}

void join_proveedor()
{
	pthread_join(prov,NULL);
}

void * _Proveedor(void *arg)
{
	int i;
	while(true)
	{
		sem_wait(&sprov);
		puts("-- Modulo Proveedor --");
		puts("Proveedor: Contactado por Comprador\n");
		if(_hizoPedido())
		{
			printf("Proveedor: Pedido recibido\n");
			peso = 0;
			for(i=0;i<N;i++)
				peso = peso + list[i].pedido;
			if(peso>0 && peso<=MAX_RUBROS)
				_prepararCamion();
			else
				_listaRechazada();
			puts("-----------------------\n");
		}
	}
}

/** Procedimientos del módulo Proveedor **/
void _prepararCamion()
{
	int i=0;
	int mayor = 4, menor = 0;
	aceptado = true;
	//_postConExito(); //AQUI
	_postValidezPedido();

	printf("Proveedor: Gestionando pedido\n");
  _llenarCamiones();
	camiones = true;
	peso_restante = peso;
	while(peso_restante>0)
	{
		i = menor;
		if(peso_restante>100 && mayor>1)
		{
			i = mayor;
			mayor--;
		}
		else
			menor++;
		if(_estadoCamion(i))
		{
			sem_post(&Camion[i]);
			if(peso < VecPeso[i])
				printf("Proveedor: Se envio camion Nro: %d con %f \n",i+1,peso);
			else
				printf("Proveedor: Se envio camion Nro: %d con %f \n",i+1,VecPeso[i]);
			peso_restante = peso_restante - VecPeso[i];
			peso = peso_restante;
		}
	}
	printf("Proveedor: Pedido en camino\n");
	sem_post(&camionEnviado);
	camiones = false;
	waitPedidoRecibido();
	printf("Proveedor: Los camiones han retornado\n");
	printf("Proveedor: Se puede recibir otro pedido\n");
	_postComprador();
	_cerrarPedido();
}

int _estadoCamion(int i)
{
	int band = 0;
	if(estado[i] == 0)
	{
		estado[i] = 1;
		band = 1;
	}
	return band;
}

// Lista de Productos Rechazada
void _listaRechazada()
{
	_postValidezPedido();
	_waitConExito(); //AQUI
	printf("Proveedor: Pedido rechazado\n");
	printf("Proveedor: Disponible para otro pedido valido\n"); //AQUI
	_postComprador();
	_cerrarPedido();

}

// Llena los camiones con el pedido
void _llenarCamiones()
{
	int i;
	for(i=0;i<N;i++)
	{
		rubros[i].pedido +=  list[i].pedido;
		rubros[i].vendido += list[i].pedido;
	}
}

// Procedimientos para otros módulos
void _postProveedor() {sem_post(&sprov);}
void _waitCamionEnviado(){sem_wait(&camionEnviado);}
int _pedidoAceptado() {return aceptado;}
void _listaP(int prod, float kg) {list[prod].pedido = kg;}
float _maxRubros() {return MAX_RUBROS;}
int _edoCamion(int c) {return estadoCamion(c);}
void _estadoC(int c, int valor) {estado[c]=valor;}
void _waitCamion(int c) {sem_wait(&Camion[c]);}
float _rubrosTotal(int id) {rubros[i].pedido;}
int _llenanCamiones() {return camiones;}
#endif
