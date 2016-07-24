#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

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
static int ELECTRICIDAD = 1;

static sem_t clientes_pref, clientes_reg, clientes_normal, clave;


/*PROCEDIMIENTOS DE CAJERAS*/
static void *caja_normal();
static void *caja_regulados();
static void *caja_pref();
static void *caja_especial();

/*INTERFAZ*/
//MODULO DE GUARDIAS
void inc_cola_regulados();

//MODULO CLIENTES
void inc_cola_normal();
void inc_cola_pref();

//SERVICIO TECNICO
void apagar_mitad_cajas();
void encender_mitad_cajas();

//INICIADORA
void init_cajeras( int n_cat, float lim_nor, float lim_reg )
{
    N_CAT = n_cat;
    LIM_NOR = lim_nor;
    LIM_REG = lim_reg;

    sem_init( &clientes_pref, 0, 0 );
    sem_init( &clientes_reg, 0, 0 );
    sem_init( &clientes_normal, 0, 0 );
    sem_init( &clave, 0, 0 );

    srand( time(NULL) );

    pthread_create( &normal1, NULL,  caja_normal, NULL  );
    pthread_create( &normal2, NULL,  caja_normal, NULL  );
    pthread_create( &normal3, NULL,  caja_normal, NULL  );
    pthread_create( &regulado1, NULL,  caja_regulados, NULL  );
    pthread_create( &regulado2, NULL,  caja_regulados, NULL  );
    pthread_create( &pref, NULL,  caja_pref, NULL  );
    pthread_create( &especial, NULL,  caja_especial, NULL  );
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
  ELECTRICIDAD = 0;
}

void encender_mitad_cajas()
{
  ELECTRICIDAD = 1;
}

/*PROCEDIMIENTOS DE CAJERAS*/

//CAJA DE PRODUCTOS NORMALES (NO REGULADOS)
static void *caja_normal()
{
  int prod, nclientes=0;
  float cant;
  while(1)
  {
    wait( &clientes_normal );
    prod = rand()%(N_CAT) + 1;
    cant = rand()%(LIM_NOR) + 1;
    if( ELECTRICIDAD && !es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
    {
      registrar( prod, cant );/*MODULO SERVICIO TECNICO*/
      nclientes++;
    }
    if( nclientes == 15 )
    {
      wait(&clave);
      nclientes = 0;
    }
  }
}

//CAJA DE PRODUCTOS REGULADOS
static void *caja_regulados()
{
    int prod, nclientes=0;
    float cant;

    while(1)
    {
        wait( &clientes_reg );
        prod = rand()%(N_CAT) + 1;
        cant = rand()%(LIM_REG) + 1;

        if( es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
        {
            registrar( prod, cant );/*MODULO SERVICIO TECNICO*/
            nclientes++;
        }

        if( nclientes == 15 )
        {
            wait(&clave);
            nclientes = 0;
        }
    }
}

//CAJA DE PRODUCTOS NORMALES PARA PUBLICO PREFERENCIAL
static void *caja_pref()
{
    int prod, nclientes=0;
    float cant;
    while(1)
    {
        wait( &clientes_pref );
        prod = rand()%(N_CAT) + 1;
        cant = rand()%(LIM_NOR) + 1;

        if( !es_regulado( prod ) /*MODULO SUPERVISOR*/ && vender( prod, cant ) /*MODULO REPONEDORES*/ )
        {
            registrar( prod, cant );/*MODULO SERVICIO TECNICO*/
            nclientes++;
        }

        if( nclientes == 15 )
        {
            wait(&clave);
            nclientes = 0;
        }
    }
}

//CAJA ESPECAL, QUE DESBLOQUEA LAS OTRAS CAJAS
static void *caja_especial()
{
    while(1) sem_post(&clave);
}
