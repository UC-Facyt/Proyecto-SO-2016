/* * Módulo Supervisor
	 * Luis Colmenarez
	 * Carlos Aponte
	 * Marien Sequera
	 * José Berenguer
	 * */
#ifndef _SUPERVISOR_H_
#define _SUPERVISOR_H_
#define N 9
#define true 1
#define false 0
#define MAX_REGULADO 288
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

//Globales
volatile int vacio = true;
volatile int open = false;
volatile int close = true;
volatile int ordenRegulado = true;
pthread_t sp;
// Prototipos de Reponedor
void _postReponedor(); // Activa el semaforo del módulo Reponedor
int _llenando(); // Funcion de reponedor.h para saber si está llenando estantes
float _rubrosRestantes(int pos);  // Consulta los Rubros en disponibilidad
float _rubrosTotal(int pos); // Consulta los Rubros totales disponibles o nó
float _rubrosRegulado();  // Consulta si hay Rubros Regulados en disponibilidad
// Prototipos de Comprador
void _postComprador(); // Activa el semaforo del módulo Comprador
void _listaPedido(int id, float cant); // Hace el pedido del Rubro id con n cantidad
// Prototipos de Guardia
void _postGuardia(); // Activa el semaforo del módulo Guardia
// Prototipos de Proveedor
float _maxRubros(); // Consulta el máximo de rubros permitidos
// Prototipos própios de supervisor
static int _listadoCierre();
int _hizoPedido(); // Retorna si se hizo un pedido a Comprador
void _cerrarPedido(); // Cierra el pedido
int _abrirMercado(); // Da la orden de abrir el mercado
int _cerrarMercado(); // Da la orden de cerrar ek mercado
int _mercadoAbierto(); // 1 = Abierto, 0 cerrado, -1 en proceso de Abrir/Cerrar
int _venderRegulado(); // Da la orden de vender regulado
int _tickets(); // Cantidad de Tickets repartidos

void _initSupervisor()
{
	pthread_create(&sp,NULL,_Supervisor,NULL);
	pthread_join(sp,NULL);
}

// Módulo Supervisor
static void * _Supervisor(void *arg) 
{
	while(true)
	{
		// Sección Critica, no protegida por que se necesita que los otros procesos pasen por acá
		// los condicionales aeguran la sincronización de los demás procesos
		puts("-- Modulo Supervisor --");
		if(!vacio)
			_postReponedor(); // Signal al Reponedor
		
		if(!open && listCierre && _listadoCierre()) // Si está cerrado y hay Lista a evaluar
		{
			// Se emite la orden a los compradores
			listCierre = false;
			puts("Procediendo a emitir orden de pedido");
			hpedido = true; // se hizo pedido
			_postComprador(); // Signal al Comprador
		}
		else if(!open && close && !hpedido && !_llenando()) // Si está cerrado y no se hará pedido
		{
			open = true; // Orden de abrir el supermercado
			puts("Dando Orden de Abrir a los Guardias");
			_postGuardia(); // Signal a los Guardias de Seguridad
		}
		if(!_llenando() && _rubrosRegulados() && !ordenRegulado && !close && open)
		{
			ordenRegulado = true; // Da la orden de vender los productos regulados
			_postGuardia(); // Signal a los Guardias de Seguridad
		}
		if(!_llenando() && !_rubrosRegulados() && !close && open)
		{
			open = false; // Orden de cerrar el supermercado
			listCierre = false; 
			_postGuardia(); // Signal a los Guardias de Seguridad
		}
		getchar();
	}
}

// -- Revisa el Listado de Cierre y llena la lista de pedidos
static int _listadoCierre()
{
	int i, pedido = false;	
	puts("Revisando Lista de Cierre");
	for(i=0;i<N;i++)
	{
		if(vacio) // Si el supermercado está vacío
		{
			_listaPedido(i,_maxRubros());
			if(i+1==N)
			{
				vacio = false;
				pedido = true;
			}
		}
		else if(_rubrosRestantes(i)<(_rubrosTotal(i)*0.40)) // Si el rubro actual tiene stock menos del 40%
		{
			_listaPedido(i,_rubrosTotal(i) - _rubrosRestantes(i)); // Se pide lo restante
			pedido = true;
		}
	}
	return pedido;
}

int _hizoPedido() {	return(hpedido); } // Retorna si se hizo el pedido
void _cerrarPedido() { hpedido = false; } // Retorna si se hizo el pedido
int _venderRegulado() {return ordenRegulado;} // Da la orden de vender productos regulados
int _tickets() {return MAX_REGULADOS(); } // Da el numero de tickets
int _abrirMercado() {return (open && close)} // Da la orden de abrir el supermercado
int _cerrarMercado() {return (!open && !close)} // Da la orden de cerrar el supermercado
int _mercadoAbierto() 
{
	int get=-1; // En proceso de Abrir/Cerrar
	if(open && !close) // Abierto
		get = true;
	else if(!open, close) // Cerrado
		get = false;
	return get;
}

#endif