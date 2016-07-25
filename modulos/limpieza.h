/* Modulo hecho por:
 * Felicia Jardim	21.480.129
 * Jesús Aguilar	21.485.074
 * Moises Astorga	21.028.351
 * Víctor Sánchez	22.518.142
 */


#ifndef LIMPIEZA_H
#define LIMPIEZA_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


//VARIABLES GLOBALES

#define N 200				//Tamaño de nuestra matriz  200mts cuadrados.
#define TVI 9999			//Tamaño de el Vector de Indices.
#define NEmpleados 4		//Cantidad de Empleados
#define NCepillos 1			//Cantidad de Cepillos
#define NMopa 2				//Cantidad de Mopas

typedef struct{
    int x,y;
}Tindice;
typedef Tindice Tindices[TVI];

typedef struct {
     char *nombre;
     int numero;
     int ini,fin;
     int x,y;
     int ii,jj,TamI,TamJ;
}Tparam;
typedef Tparam Tvector[4];//Parametros de los hilos.

static sem_t mutex,Escoba,Mopa,indic;//Semaforos para controlar la Mopa,Escoba y Matriz; Tambien para controlar los indices compartidos.
static int cep = NCepillos,mop = NMopa;

typedef pthread_t Templeados[NEmpleados];// Numero de Empleados,Estructura de hilos


static int Tmatriz[N][N];//Recurso critico (area del Mercado)
static int count=0;		//recurso compartido contador para ver los segmentos sucios restantes lol
Tindices indice;		//Recurso Compartido para mover el empleado que use alguna herramientra de limpieza
static int TamUso=0;	//Indicara si hay peticiones  de herramientas de limpieaza en cola [!= 0 hay peticiones]
static int band;

pthread_t thread1;
Templeados empleado;	//Vector de Treads
Tvector param;			//Vector de los parametros que tendran los Empleados.

/*Procedimientos de Limpieza*/
static void * _empleadoProc(Tvector *n);// Proceso para la Creacion de los hilos.
static void *_ensuciarMercado();		//Procedimiento para ensuciar el Mecado.
static void _limpiarMercado(int i,int j,int NumEmpleado,char *nombre);//Procedimiento que limpia El mercado segun toque.
static void _mostrarMercado(int tipo);	//Muestra el Mercado.O imprime en un archivo.


//INICIADORA
void init_limpieza()
{
    int i;
    band=0;

	//Inicializacion de los Semaforos
    sem_init(&Escoba,0,1);
    sem_init(&Mopa,0,3);
    sem_init(&mutex,0,1);
    sem_init(&indic,0,1);
	//////////////////////////////////
	
	//Inicializacion de el Vector de parametros de los Empleados.
    param[0].nombre="Víctor S.";
    param[0].numero=0;
    param[0].ini=0;
    param[0].fin=N/2;

    param[1].nombre="Jesús A.";
    param[1].numero=1;
    param[1].ini=0;
    param[1].fin=N;

    param[2].nombre="Moises A.";
    param[2].numero=2;
    param[2].ini=(N/2);
    param[2].fin=(N/2);

    param[3].nombre="Felicia J.";
    param[3].numero=3;
    param[3].ini=(N/2);
    param[3].fin=N;
	/////////////////////////////////
	
    //Creacion del hilo que llena la matriz
    if(pthread_create(&thread1,NULL,_ensuciarMercado,NULL) ) perror("ERROR_ Creando Hilo para llenar Matriz\n");

    //Mostrar Matriz Creada.
    _mostrarMercado(1);

	//Ciclo donde se crea los 4 hilos para los empleados.
	for(i=0; i<4; i++)
	{
       	if( pthread_create(&empleado[i],NULL,(void*)_empleadoProc,&param[i])!=0) perror("ERROR_ Creando Hilo para Empleados");

	}
}

//DESINICIADORA
void join_limpieza()
{
    pthread_join(thread1,NULL);
    pthread_join(empleado[0],NULL);
    pthread_join(empleado[1],NULL);
    pthread_join(empleado[2],NULL);
    pthread_join(empleado[3],NULL);
}


/*Procedimientos de Limpieza*/
static void * _empleadoProc( Tvector *n){

	int i=0,j=0;

    while(count>=1){// mientras la matriz este sucia, (count >=1 los hilos se ejecutaran)

        if((*n)->ini==0){//aqui modifico los paramaetros de los hilos para asignar su ruta de limpieza

            if((*n)->fin!=N){
                (*n)->TamI=(*n)->fin;
                (*n)->TamJ=(*n)->fin;
               (*n)->ii=0;
                (*n)->jj=0;

            }else{

                (*n)->ii=0;
                (*n)->jj=N/2;
                (*n)->TamJ=N;
                (*n)->TamI=N/2;

            }//end if-else

        }else{

            if((*n)->fin!=N/2){
                (*n)->TamI=(*n)->fin;
                (*n)->TamJ=(*n)->fin;
                (*n)->ii=N/2;
                (*n)->jj=N/2;

            }else{
                (*n)->ii=(*n)->fin;
                (*n)->jj=0;
                (*n)->TamJ=N/2;
                (*n)->TamI=N;

            }//end if-else
        }//end if-else
             for(i=(*n)->ii;i<(*n)->TamI;i++){
                for(j=(*n)->jj;j<(*n)->TamJ;j++){

                  if(Tmatriz[i][j]>0){//verifico si Tmatriz [i][j] esta sucio (>0) llamo a un procedimiento de limpieza
                            _limpiarMercado(i,j,(*n)->numero,(*n)->nombre);//procedimiento de limpieza donde en el uso los semaforos y mando el nombre y numero del empleado que esta limpiando
                  }//end if

                }//end for*
            }//end for*/
    }//end while

    return 0;
}// end void * _empleadoProc(void *n);

/*
 * Procedimiento para SIMULAR que el cliente ensucia el Mercado.
 * */
static void *_ensuciarMercado(){

    int i,j,aux,ind=1;
    srand(time(NULL));

    if(band<5){

        sem_wait(&mutex);
        while(ind<=10){
            i=rand()%N;
            j=rand()%N;
            if(Tmatriz[i][j]==0){

                aux=rand()%4;

                while(aux==0){
                    aux=rand()%4;
                }//end while

                Tmatriz[i][j]=aux;
                count++;
                ind++;
            }//end if

        }//end while
        sem_post(&mutex);
        band++;
    }
    return 0;

}//end void llenar_matriz();

/*
 * Muestra el estado de el Mercado[Matriz] 1 para generar un Salida por archivo y 2 para Mostrar por Pantalla.
 * */
static void _mostrarMercado(int tipo){
    int i,j;
    FILE *out;

    switch(tipo){

        case 1:
            out= fopen("salida.dat","a");

            if(out!=NULL){

                for(i=0;i<N;i++){
                    for(j=0;j<N;j++){

                       fprintf(out,"[%i]",Tmatriz[i][j]);

                    }//end for*
                    fprintf(out,"\n");
                }//end for
                fprintf(out,"\n");
            }
            fclose(out);
        break;

        case 2:
            for(i=0;i<N;i++){
                    for(j=0;j<N;j++){

                       printf("[%i]",Tmatriz[i][j]);

                    }//end for*
                    printf("\n");
                }//end for
                printf("\n");
        break;
    }//end switch
}//end _mostrarMercado

/*
 * Procedimiento que limpia el Mercado, de acuerdo a id de el empleado.
 * */
static void _limpiarMercado(int i,int j,int NumEmpleado,char *nombre){
    //este procedimiento solo trabaja con 3 semaforos uno para la matriz,otro para la escoba y otro para la mopa !
    switch(Tmatriz[i][j]){

        case 1:

            printf("->>Se debe pasar solo la Escoba.\n");

            if(cep>0){
                sem_wait(&Escoba);
                cep--;
                sem_wait(&mutex);
                printf("Empleado  %i %s \n",NumEmpleado,nombre);
                printf("->Limpiando con la Escoba.\n");
                Tmatriz[i][j]=0;
                count--;
                while(TamUso>0){

                    printf("Empleado  %i %s \n",NumEmpleado,nombre);
                    printf("->Limpiando con la Escoba en el cuadrante [%i-%i].\n",indice[TamUso].x,indice[TamUso].y);
                    Tmatriz[indice[TamUso].x][indice[TamUso].y]=0;

                    count--;
                     sem_wait(&indic);
                     TamUso--;
                     sem_post(&indic);
                }//end if
                _mostrarMercado(1);
                sem_post(&mutex);
                sem_post(&Escoba);
                cep++;
            }else{
                sem_wait(&indic);
                TamUso++;
                indice[TamUso].x=i;
                indice[TamUso].y=j;
                sem_post(&indic);
            }//end if-else


        break;

        case 2:
            printf("->>Se debe pasar solo la Mopa.\n");
            if(mop>0){
                sem_wait(&Mopa);
                mop--;
                sem_wait(&mutex);
                printf("Empleado  %i %s \n",NumEmpleado,nombre);
                printf("->Limpiando con la Mopa.\n");
                Tmatriz[i][j]=0;
                count--;
                while(TamUso>0){
                        printf("Empleado  %i %s \n",NumEmpleado,nombre);
                        printf("->Limpiando con la Mopa en el cuadrante [%i-%i].\n",indice[TamUso].x,indice[TamUso].y);
                        Tmatriz[indice[TamUso].x][indice[TamUso].y]=0;
                        count--;
                        sem_wait(&indic);
                        TamUso--;
                        sem_post(&indic);
                }//end if
                _mostrarMercado(1);
                sem_post(&mutex);
                sem_post(&Mopa);
                mop++;
            }else{
                sem_wait(&indic);
                TamUso++;
                indice[TamUso].x=i;
                indice[TamUso].y=j;
                sem_post(&indic);

            }//end if-else



        break;

        case 3:
            printf("->>Se debe pasar la Escoba y la Mopa.\n");
            if(cep>0 && mop>0){
                sem_wait(&Escoba);
                cep--;
                sem_wait(&Mopa);
                mop--;
                sem_wait(&mutex);
                printf("Empleado  %i %s \n",NumEmpleado,nombre);
                printf("->Limpiando con la Escoba y la Mopa.\n");
                Tmatriz[i][j]=0;
                count--;
                while(TamUso>0){
                        printf("Empleado  %i %s \n",NumEmpleado,nombre);
                        printf("->Limpiando con la Escoba en el cuadrante [%i-%i].\n",indice[TamUso].x,indice[TamUso].y);
                        Tmatriz[indice[TamUso].x][indice[TamUso].x]=0;
                        count--;
                        sem_wait(&indic);
                        TamUso--;
                        sem_post(&indic);

                    }//end if
                _mostrarMercado(1);
                sem_post(&mutex);
                sem_post(&Mopa);
                mop++;
                sem_post(&Escoba);
                cep++;

            }else{
                sem_wait(&indic);
                TamUso++;
                indice[TamUso].x=i;
                indice[TamUso].y=j;
                sem_post(&indic);

            }//end if-else


        break;
    }//end switch

}//end void _limpiarMercado
#endif
