#ifndef MERCADEO_H
#define MERCADEO_H
#include "cajeras.h"
#include "servicio_tecnico.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 		1000
#define TRUE 	1
#define FALSE	0

/* COSAS QUE FALTAN: MIRAR ABAJO POR FAVOR...	*/
/* es_regulado() necesitamos saber que modulo verifica si un producto es regulado " NO SE QUIEN LO HACE"	*/
/* DEBO SABER SI HUBO ALGUNA INTERRUPCION O NINGUNA EN LA SEMANA*/

/*****************************/ 

static sem_t estado;

pthread_t merc;

void* init_MERCADEO() {

	pthread_create(&merc, NULL, departamentoMercadeo, NULL);

}

static void* departamentoMercadeo(void *data) {

	while(TRUE) {

		/* Monkeys making coffee 	*/
		/* until the days end    	*/
		/* Make the day report 		*/

		/* If is the last week day 	*/
		/* make the week report 	*/
	}
}

/*****************************/ 

/******************* " POSIBLES "ESTRUCTURAS QUE UTILIZAREMOS EN EL MODULO DE MERCADEO***********************************/
typedef struct
{
    int id; // identificador unico para el producto.......
    int cantidad; // CANTIDAD DE COMPRADA DEL PRODUCTO...
    float prob = 100; // PROBABILIDAD DE VENTA
} producto;
typedef struct
{
	int dia; // DIA DE LA SEMANA..
	int cant_prod_vendidos; // CANTIDAD DE PRODUCTOS VENDIDOS EN EL DIA...
	int prod_mas_vendido; // PRODUCTO QUE MAS SE VENDIO EN EL DIA...
	int prod_menos_vendido; // PRODUCTO QUE MENOS SE VENDIO EN EL DIA....
	int interrupciones; // INDICA SI HUBO O NO INTERRUPCION EN EL DIA (1= SI  0=NO)...
	cajera *ventas_cajeras[5]; // APUNTA A LA ESTRUCTURA DE LAS CAJERAS PARA SABER LA INFORMACION DE LAS MISMAS..
	producto proba[9];
} report; // ESTRUCTURA QUE UTILIZAREMOS PARA ESCRIBIR LOS DATOS DEL REPORTE DIARIO Y SEMANAL....

typedef struct
{
    producto producto_vendido[N];
    int cant_prod_regulados;
    int cant_prod_no_regulados;
    int cajera_id = -1;
}cajera; // Estructura para la lista de productos vendidos en el dia POR LA CAJERA....

typedef cajera lista_ventas_cajera[5]; // lista para los productos vendidos en el dia.

/*****************************************************************************************************************/

/******************** VARIABLES GLOBALES ************************************************************/
int cont1=0; // CONTADOR GLOBAL PARA MOVERME EN EL VECTOR DE REPORTES POR DIA........
int cont2=0;
typedef report reporte[8]; // VECTOR QUE SIMULA " LOS DIAS DE LA SEMANA ", LA CASILLA 8 SERVIRA PARA COLOCAR EL REPORTE GENERAL DE LA SEMANA..

/*****************************************************************************************************************/

/**************** ENCABEZADOS DE LOS PROCEDIMIENTOS Y (ó) FUNCIONES QUE SE USARAN *************************************************************************************************/
void registro(int prod, float cant,int id_cajera);// venta que realiza una cajera
void reporteD(*reportes reporte[cont1]); // reporte del dia...
void reporteS(*reportes reporte[cont1]);
/*****************************************************************************************************************/
/**** Simulacion de los productos vendidos en un dia con la informacion que pasa cajeras.h  ****/
void registro(int prod, float cant, int id_cajera)
{
	/*FALTA SABER QUE CAJERA HIZO LA VENTA DEL PRODUCTO, PARA ASI TAMBIEN TENER CONOCIMIENTO SI EL PRODUCTO ES REGULADO O NO*/
	int j=0;
	int band 0;
	while(band != 1)
	{
		if(lista_ventas_cajera[j].cajera_id == id_cajera) // verifico si la cajera ya esta en el sistema para no volverla a anotar; solo anotar los productos vendidos por ella...
		{
		    lista_ventas_cajera[j].producto_vendido[cont2].id = prod;
		    lista_ventas_cajera[j].producto_vendido[cont2].cantidad = cant; 
		    band = 1;                  
		}
		else
		{
		   if(lista_ventas_cajera[j].cajera_id == -1)
		   {
		       lista_ventas_cajera[j].cajera_id = id_cajera;
		       lista_ventas_cajera[j].producto_vendido[cont2].id = prod;
		       band = 1;
		   }
		}
		j++;
    }
    int i=0;
    while(band != 1)
    {
		if(reportes[cont1].ventas_cajeras[j]->producto_vendido[i].id == prod) // guardo el reporte diario por cajera, para luego usar estos datos para el reporte semanal....
		{
			reportes[cont1].ventas_cajeras[j]->producto_vendido[i].cantidad = reportes[cont1].ventas_cajeras[j]->producto_vendido[i].cantidad + cant;                                                               
			band = 1; 
		}
		else
		{
			reportes[cont1].ventas_cajeras[j]->producto_vendido[i].id = prod;
			reportes[cont1].ventas_cajeras[j]->producto_vendido[i].cantidad = cant;
			band = 1;   
		}
		i++;
    }
    cont2++;
}

/*** Reporte del dia ***/
void reporteD(*reportes reporte[cont1]) // recibe la lista de productos vendidos en el dia, y el arreglo de productos
{
    int prod[10]; // arreglo para guardar la suma total de cada producto
    int max=0, min=999999;
    int pos_max, pos_min, total=0, total1=0;
    int j =0;
    int i = 0;
    if(cont1 == 7)
             cont1 = 0;
             
    for (i = 0; i < 10; i++) 
    {
    	prod[i]=0;
    }
    i=0;
    while(lista_ventas[j].producto_vendido[i].id !=-1) // mientras haya productos en la lista compara y suma
    {
        prod[lista_ventas[j].producto_vendido[i].id]+=lista_ventas[j].producto_vendido[i].cantidad; // va sumando las cantidades de cada producto y guarda el total
        i++;
    }
    
	
    for (i = 0; i < 9; i++) // busca el maximo y el minimo de productos vendidos
    {
		total1 = total1 + prod[i];
        if (prod[i]>max)
        {
            max=prod[i];
            pos_max=i;
        }
        if (prod[i]<min)
        {
            min=prod[i];
            pos_min=i;
        }
        for (i = 0; i < 9; i++)
		{
			reporte[cont1].proba[i]=(prod[i]/total1)*100; //guarda las probabilidades de venta del dia siguiente
		}
		
        total+=prod[i]; // saco el total de productos vendidos....
        reporte[cont1].dia = cont1; // numero de dia...
        reporte[cont1].cant_prod_vendidos = total;
        reporte[cont1].prod_mas_vendido = prod[pos_max]; 
        reporte[cont1].prod_menos_vendido = prod[pos_min];
        
    }
    cont1++;
    cont2 = 0;
}
void reporteS(*reportes reporte[cont1])
{
     int i, z, t, j;
     reporte[cont1-1].interrupcion = nosVolvimosLocos(); //verifico si hubo corte electrico...
     z=0;
     /*verifico por dia las ventas de los productos regulados o no, para hacer el reporte semanal */
     for(i=0; i < 7; i++) // ciclo para moverme en el reporte por dia
     {
             j=0;
             while(reportes[i].ventas_cajeras[i]->cajera_id != -1) // ciclo para moverme en las ventas que hicieron cada cajera....
             {
                t=0;
                while(reporte[i].ventas_cajeras[j].producto_vendido[t].id != -1) // ciclo para saber y sumar los productos vendidos...
                {
                    if(!es_regulado(reporte[i].ventas_cajeras[j].producto_vendido[t].id)) // debo saber si el producto es regulado o no, para sacar el total de productos regulados y no regulados vendidos por la cajera...
                    {
                       reporte[cont1].ventas_cajeras[z].cant_prod_no_regulados= reporte[cont1].ventas_cajeras[z].cant_prod_no_regulados + reporte[i].ventas_cajeras[j].producto_vendido[t].cantidad;     
                    }
                    else
                    {
                       reporte[cont1].ventas_cajeras[z].cant_prod_regulados= reporte[cont1].ventas_cajeras[z].cant_prod_regulados + reporte[i].ventas_cajeras[j].producto_vendido[t].cantidad;
                    }
                    t++;
                }
                j++
                                                                                                                   
             }
             z++;
              
     }
     cont1 =0;
          
}

#endif
