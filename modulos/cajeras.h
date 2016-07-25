#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "estantes.h"

/*
  AUTORES:
  GONZALEZ FELIX
  RODRIGUEZ VIVIANA
  CANTERO CRISTIAN
  GONCALVES MARIANGELA
*/

//Puse estas mientras desorrollan los demas modulos
int es_regulado( int prod )
{
    if(rand()%2==0) return (1);
    else return (0);
}

void registrar( int a, float b, int idcajera )
{
}

/*NOTAS

  INTERFACES NECESARIAS DE OTROS MODULOS

  //SUPERVISOR
  es_regulado( int prod ): prod es el numero que representa un producto, si es regulado retorna verdad y en caso
  contrario falso.

  //REPONEDORES
  vender( int prod, float cant ): prod es el numero que representa un producto y cant la cantidad de ese producto
                                  que quiere comprar un producto. dado esto deberian "quitar" esa cantidad de ese
                                  producto del estante. Retornar verdad si hay suficiente del producto y falso en
                                  caso contrario.
  //SERVICIO TECNICO O MERCADEO
  registrar( int prod, float cant ): prod es el numero que representa un producto y cant la cantidad de ese producto
                                  que quiere comprar un producto, registra que se vendio esa cantidad de ese producto.
*/

//VARIABLES GLOBALES
pthread_t normal1, normal2, normal3;
pthread_t regulado1, regulado2;
pthread_t pref;
pthread_t especial;

static int N_CAT, LIM_NOR, LIM_REG;

static sem_t clientes_pref, clientes_reg, clientes_normal, dar_clave, pedir_clave;
static sem_t func[7];

/*PROCEDIMIENTOS DE CAJERAS*/
static void *caja_normal( void *arg );
static void *caja_regulados( void *arg );
static void *caja_pref( void *arg );
static void *caja_especial( void *arg );

/*INTERFAZ*/
//MODULO DE GUARDIAS
void inc_cola_regulados();

//MODULO CLIENTES
void inc_cola_normal();
void inc_cola_pref();

//SERVICIO TECNICO
void apagar_mitad_cajas();
void encender_mitad_cajas();
void cerrar_cajas();
void abrir_cajas();

//ESTANTES
int vender( int prod, int cant );

//INICIADORA
void init_cajeras( int n_cat, float lim_nor, float lim_reg )
{
    N_CAT = n_cat;
    LIM_NOR = lim_nor;
    LIM_REG = lim_reg;
    int vect[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

    sem_init( &clientes_pref, 0, 0 );
    sem_init( &clientes_reg, 0, 0 );
    sem_init( &clientes_normal, 0, 0 );
    sem_init( &dar_clave, 0, 0 );
    sem_init( &pedir_clave, 0, 0 );

    srand( time(NULL) );

    pthread_create( &normal1, NULL,  caja_normal, (void *)  &vect[1] );
    pthread_create( &normal2, NULL,  caja_normal, (void *)  &vect[2] );
    pthread_create( &normal3, NULL,  caja_normal, (void *)  &vect[3] );
    pthread_create( &regulado1, NULL,  caja_regulados, (void *)  &vect[4] );
    pthread_create( &regulado2, NULL,  caja_regulados, (void *)  &vect[5] );
    pthread_create( &pref, NULL,  caja_pref, (void *)  &vect[6]  );
    pthread_create( &especial, NULL,  caja_especial, (void *)  &vect[7]  );
}

//DESINICIADORA
void join_cajeras()
{
    pthread_join(normal1, NULL);
    pthread_join(normal2, NULL);
    pthread_join(normal3, NULL);
    pthread_join(regulado1, NULL);
    pthread_join(regulado2, NULL);
    pthread_join(pref, NULL);
    pthread_join(especial, NULL);
}

//MODULO DE GUARDIAS
void inc_cola_regulados()
{
    sem_post(&clientes_reg);
}

//MODULO CLIENTES
void inc_cola_normal()
{
    sem_post( &clientes_normal );
}
void inc_cola_pref()
{
    sem_post( &clientes_pref );
}

//SERVICIO TECNICO
void apagar_mitad_cajas()
{
    wait( &func[1] );
    wait( &func[2] );
    wait( &func[3] );
}

void encender_mitad_cajas()
{
    sem_post( &func[1] );
    sem_post( &func[2] );
    sem_post( &func[3] );
}

void cerrar_cajas()
{
    wait( &func[1] );
    wait( &func[2] );
    wait( &func[3] );
    wait( &func[4] );
    wait( &func[5] );
    wait( &func[6] );
    wait( &func[7] );
}

void abrir_cajas()
{
  sem_post( &func[1] );
  sem_post( &func[2] );
  sem_post( &func[3] );
  sem_post( &func[4] );
  sem_post( &func[5] );
  sem_post( &func[6] );
  sem_post( &func[7] );
}

//ESTANTES
int vender( int prod, int cant )
{
    return( /*Comunicacion_Cajera_Estante( prod, (float) cant ) == 1*/ (rand()%2 == 0) ? 1 : 0 );
}

/*PROCEDIMIENTOS DE CAJERAS*/

//CAJA DE PRODUCTOS NORMALES (NO REGULADOS)
void *caja_normal( void *arg )
{
    int prod, nclientes = 0;
    int i = *((int *) arg);
    float cant;
    while(1)
    {
        wait( &func[i] );
        wait( &clientes_normal );
        prod = rand()%(N_CAT) + 1;
        cant = rand()%(LIM_NOR) + 1;

        if( !es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
        {
            printf("Venta en caja normal %d\n", i );
            sleep(1);
            registrar( prod, cant, i );/*MODULO SERVICIO TECNICO*/
            nclientes++;
        }

        if( nclientes == 15 )
        {
            sem_post(&pedir_clave);
            printf("Bloquea en caja  normal %d\n", i );
            sleep(1);
            wait(&dar_clave);
            printf("Desbloquea en caja normal %d\n", i );
            sleep(1);
            nclientes = 0;
        }

        sem_post( &func[i] );
    }
}

//CAJA DE PRODUCTOS REGULADOS
void *caja_regulados( void *arg )
{
    int prod, nclientes = 0;
    int i = *((int *) arg);
    float cant;
    while(1)
    {
        wait( &func[i] );
        wait( &clientes_reg );
        prod = rand()%(N_CAT) + 1;
        cant = rand()%(LIM_REG) + 1;

        if( es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
        {
            printf("Venta en caja regulado %d\n", i );
            sleep(1);
            registrar( prod, cant, i );/*MODULO SERVICIO TECNICO*/
            nclientes++;
        }

        if( nclientes == 15 )
        {
            sem_post(&pedir_clave);
            printf("Bloquea en caja regulado %d\n", i );
            sleep(1);
            wait(&dar_clave);
            printf("Desbloquea en caja regulado %d\n", i );
            sleep(1);
            nclientes = 0;
        }

        sem_post( &func[i] );
    }
}

//CAJA DE PRODUCTOS NORMALES PARA PUBLICO PREFERENCIAL
void *caja_pref( void *arg )
{
    int prod, nclientes = 0;
    float cant;
    int i = *((int *) arg);
    while(1)
    {
        wait( &func[i] );
        wait( &clientes_pref );
        prod = rand()%(N_CAT) + 1;
        cant = rand()%(LIM_NOR) + 1;

        if( !es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
        {
            printf("Venta en caja Pref\n");
            sleep(1);
            registrar( prod, cant, i );/*MODULO SERVICIO TECNICO*/
            nclientes++;
        }

        if( nclientes == 15 )
        {
            sem_post(&pedir_clave);
            printf("Bloquea en caja Pref\n");
            sleep(1);
            wait(&dar_clave);
            printf("Desbloquea en caja Pref\n");
            sleep(1);
            nclientes = 0;
        }

        sem_post( &func[i] );
    }
}

//CAJA ESPECIAL, QUE DESBLOQUEA LAS OTRAS CAJAS
void *caja_especial( void *arg )
{
    int i = *((int *) arg);
    while(1)
    {
        wait( &func[i] );
        wait(&pedir_clave);
        sem_post(&dar_clave);
        sem_post( &func[i] );
    }
}
