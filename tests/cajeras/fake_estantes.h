#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


int vender( int prod, float cant )
{
    return rand() % 2;
}
