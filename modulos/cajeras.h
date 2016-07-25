/* Modulo Cajeras
 * AUTORES:
 * GONZALEZ FELIX
 * RODRIGUEZ VIVIANA
 * CANTERO CRISTIAN
 * GONCALVES MARIANGELA
 */

#ifndef CAJERAS_H
#define CAJERAS_H

/*#ifndef UNIT_TESTING
#include "estantes.h"
#include "mercadeo.h"
#include "supervisor.h"
#endif*/
 //fakie

#include "../fakies/fake_supervisor.h"
#include "../fakies/fake_estantes.h"
#include "servicio_tecnico.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


//Puse estas mientras desorrollan los demas modulos
// int es_regulado( int prod )
// {
//     if(rand()%2==0) return (1);
//     else return (0);
// }
//
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
static pthread_t normal1, normal2, normal3;
static pthread_t regulado1, regulado2;
static pthread_t pref;
static pthread_t especial;

volatile static int N_CAT, LIM_NOR, LIM_REG;

static sem_t clientes_pref, clientes_reg, clientes_normal, dar_clave, pedir_clave;
sem_t func[7], permiso_normal, enc[7];
static int vect[7], index;

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
// int vender( int prod, int cant );

//INICIADORA
void init_cajeras( int n_cat, float lim_nor, float lim_reg )
{
    N_CAT = n_cat;
    LIM_NOR = lim_nor;
    LIM_REG = lim_reg;

    for (index = 0; index < 7; index++){
        vect[index] = index;
        printf("INICIALIZANDO %d\n", index);
        sem_init( &func[index], 0, 1);

        sem_init( &enc[index], 0, 1);
    }

    sem_init( &clientes_pref, 0, 0 );
    sem_init( &clientes_reg, 0, 0 );
    sem_init( &clientes_normal, 0, 0 );
    sem_init( &dar_clave, 0, 0 );
    sem_init( &pedir_clave, 0, 0 );

    sem_init( &permiso_normal, 0, 1 );

    srand( time(NULL) );

    pthread_create( &normal1,   NULL,  caja_normal,    (void*) &vect[0] );
    pthread_create( &normal2,   NULL,  caja_normal,    (void*) &vect[1] );
    pthread_create( &normal3,   NULL,  caja_normal,    (void*) &vect[2] );
    pthread_create( &regulado1, NULL,  caja_regulados, (void*) &vect[3] );
    pthread_create( &regulado2, NULL,  caja_regulados, (void*) &vect[4] );
    pthread_create( &pref,      NULL,  caja_pref,      (void*) &vect[5] );
    pthread_create( &especial,  NULL,  caja_especial,  (void*) &vect[6] );

    printf("Inicializacion correcta del modulo Cajeras \n");
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
    printf("Apagandooo \n");

    int value, i;
    for (i = 0; i < 3; i++) {
        sem_getvalue(&func[i], &value);
        printf("Valor semaforo %d = %d\n", i, value);
    }


    sem_wait( &enc[0] );

    printf("Apagando 1\n ");



    sem_wait( &enc[1] );
    sem_wait( &enc[2] );
    printf("Semaforitos Apagados \n");
}

void encender_mitad_cajas()
{
    sem_post( &enc[0] );
    sem_post( &enc[1] );
    sem_post( &enc[2] );
}

void cerrar_cajas()
{
    sem_wait( &enc[0] );
    printf("apague 0\n");
    sem_wait( &enc[1] );
    printf("apague 1\n");
    sem_wait( &enc[2] );
    printf("apague 2\n");
    sem_wait( &enc[3] );
    printf("apague 3\n");
    sem_wait( &enc[4] );
    printf("apague 4\n");
    sem_wait( &enc[5] );
    printf("apague 5\n");
    sem_wait( &enc[6] );
    printf("apague 6\n");
}

void abrir_cajas()
{
    sem_post( &enc[0] );
    sem_post( &enc[1] );
    sem_post( &enc[2] );
    sem_post( &enc[3] );
    sem_post( &enc[4] );
    sem_post( &enc[5] );
    sem_post( &enc[6] );
    printf("Estoy subiendo las barras\n");
}

//ESTANTES
// int vender( int prod, int cant )
// {
//     return( /*Comunicacion_Cajera_Estante( prod, (float) cant ) == 1*/ (rand()%2 == 0) ? 1 : 0 );
// }

/*PROCEDIMIENTOS DE CAJERAS*/

//CAJA DE PRODUCTOS NORMALES (NO REGULADOS)
static void *caja_normal( void *arg )
{
    int prod, nclientes = 0;
    int i = *((int *) arg);
    printf("Caja normal %d inicializada! \n", i);
    float cant;
    while(1)
    {
        sem_wait(&enc[i]);
        sem_post(&enc[i]);

        sem_wait( &func[i] );
            sem_wait( &clientes_normal );
            prod = rand()%(N_CAT) + 1;
            cant = rand()%(LIM_NOR) + 1;

            if ( !es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
            {
                printf("-- -- Venta en caja normal %d \n", i);
                // sleep(1);
                registrar( prod, cant, i );/*MODULO SERVICIO TECNICO*/
                nclientes++;
            }

            if( nclientes == 15 )
            {
                sem_post(&pedir_clave);
                printf("Bloquea en caja  normal %d\n", i );
                // sleep(1);
                sem_wait(&dar_clave);
                printf("Desbloquea en caja normal %d\n", i );
                // sleep(1);
                nclientes = 0;
            }
        sem_post( &func[i] );
    }
}

//CAJA DE PRODUCTOS REGULADOS
static void *caja_regulados( void *arg )
{
    int prod, nclientes = 0;
    int i = *((int *) arg);
    printf("Caja regulada %d inicializada! \n", i);
    float cant;
    while(1)
    {
        sem_wait(&enc[i]);
        sem_post(&enc[i]);

        sem_wait( &func[i] );
            sem_wait( &clientes_reg );
            prod = rand()%(N_CAT) + 1;
            cant = rand()%(LIM_REG) + 1;

            if( es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
            {
                printf("Venta en caja regulado %d\n", i );
                // sleep(1);
                registrar( prod, cant, i );/*MODULO SERVICIO TECNICO*/
                nclientes++;
            }

            if( nclientes == 15 )
            {
                sem_post(&pedir_clave);
                printf("Bloquea en caja regulado %d\n", i );
                // sleep(1);
                sem_wait(&dar_clave);
                printf("Desbloquea en caja regulado %d\n", i );
                // sleep(1);
                nclientes = 0;
            }

        sem_post( &func[i] );
    }
}

//CAJA DE PRODUCTOS NORMALES PARA PUBLICO PREFERENCIAL
static void *caja_pref( void *arg )
{
    int prod, nclientes = 0;
    float cant;
    int i = *((int *) arg);
    printf("Caja preferencial %d inicializada! \n", i);
    while(1)
    {
        sem_wait(&enc[i]);
        sem_post(&enc[i]);

        sem_wait( &func[i] );
            sem_wait( &clientes_pref );
            prod = rand()%(N_CAT) + 1;
            cant = rand()%(LIM_NOR) + 1;

            if( !es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
            {

                printf("Venta en caja Pref\n");
                // sleep(1);
                registrar( prod, cant, i );/*MODULO SERVICIO TECNICO*/
                nclientes++;
            }

            if( nclientes == 15 )
            {
                sem_post(&pedir_clave);
                printf("Bloquea en caja Pref\n");
                // sleep(1);
                sem_wait(&dar_clave);
                printf("Desbloquea en caja Pref\n");
                // sleep(1);
                nclientes = 0;
            }
        sem_post( &func[i] );
    }
}

//CAJA ESPECIAL, QUE DESBLOQUEA LAS OTRAS CAJAS
static void *caja_especial( void *arg )
{
    int i = *((int *) arg);
    printf("Caja especial %d inicializada! \n", i);
    while(1)
    {
        sem_wait(&enc[i]);
        sem_post(&enc[i]);

        sem_wait( &func[i] );
            sem_wait(&pedir_clave);
            sem_post(&dar_clave);
        sem_post( &func[i] );
    }
}

#endif
