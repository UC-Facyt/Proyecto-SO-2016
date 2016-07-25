/** Modulo: Compradores
	* Realizado por Módulo Supervisor y Proveedor
  * */
#ifndef _COMPRADOR_H_
#define _COMPRADOR_H_

#define true 1
#define false 0
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

pthread_t comp[6]; // Hilo del Módulo
// Semáforos
sem_t Comprador;
sem_t validezPedido;
sem_t pedidoRecibido;
sem_t conExito;
// Prototipos de Supervisor
int _hizoPedido();
// Prototipos de Reponedores
int _llenanCamiones();
float _rubrosDeposito(int id);
void _descargaDeposito(int pos, float cant);

// Prototipos de Proveedores
void _postProveedor();
int _pedidoAceptado();
void _waitCamionEnviado();
float _rubrosTotal(int id);
int _edoCamion(int camion);
void _estadoC(int camion, int valor);
void _waitCamion(int camion);
void _listaP(int prod, float kg);
// Prototipos Propios
void _postComprador();
void _postConExito();
void _postValidezPedido();
void _waitPedidoRecibido();
void _listaPedido(int prod, float kg);

void _initComprador()
{
	int i;
	// Inicializando Semáforos
	sem_init(&validez_pedido, 0, 0);
	sem_init(&pedido_recibido, 0, 0);
	sem_init(&conExito, 0, 0);

	// Creando procesos compradores (6)
	for(i=0;i<6;i++)
		pthread_create(&comp[i],NULL,_Comprador,&i);
	for(i=0;i<6;i++)
		pthread_join(&comp[i],NULL);
}

// Procedimiento principal del módulo
void * _Comprador(void *arg)
{
	int i;
	int n = *((int*)arg);
	while(true)
	{
		// Seccion Crítica
		sem_wait(Comprador); // Wait para proteger la sección
		puts("-- Modulo Comprador --");
		if(_hizoPedido() && !_llenanCamiones()) // Si se autoriza el pedido y aun no llenan los camiones
		{
			printf("Comprador %d: Se llena el pedido\n", n+1);
			_postProveedor(); // Signal a Proveedor
			printf("Comprador %d: El comprador contacta al proveedor\n",n+1);
			sem_wait(&validezPedido); // Espera la confirmacion
			printf("Comprador %d: Se verifica que el pedido haya sido aceptado\n",n+1);
			sem_wait(&conExito); // Wait
			if(_pedidoAceptado()) // Si el pedido fué aceptado
			{
				printf("Comprador %d: Pedido aceptado\n",n+1);
				_waitCamionEnviado();
				printf("Comprador %d: Se espera a que los camiones lleguen\n",n+1);
				for(i=0;i<5;i++)
					if(!_edoCamion(i))
					{
						_estadoC(i,0);
						_waitCamion(i);
						printf("Comprador %d: El camion %d llego\n",n+1,i+1);
					}
				printf("Comprador %d: Pedido recibido sin inconvenientes\n", n+1);
				printf("Comprador %d: Camiones Recibidos\nDescargando Mercancia",n+1);
				sem_post(&pedidoRecibido);
				for(i=0;i<N;i++) // Descargando los Camiones en los Depósitos
					_descargaDeposito(i,_rubrosDeposito(i) + _rubrosTotal(i));
				// Fin de la seccion critica
			}
		}
		puts("-----------------------\n");
	}
}

void _postComprador() {sem_post(Comprador);} // Signal al Módulo
void _postConExito() {sem_post(conExito);}
void _waitConExito(){sem_wait(&conExito);}
void _postValidezPedido() {sem_post(validezPedido);}
void _waitPedidoRecibido() {sem_wait(pedidoRecibido);}
void _listaPedido(int prod, float kg)
{
	_listaP(prod,kg);
}
