#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


int es_regulado( int prod )
{
    if(rand()%2==0){
        // printf("Producto REGULADo\n");
        return (1);
    }else{
        // printf("Producto NO REGULADO\n");
        return (0);
    }
}
