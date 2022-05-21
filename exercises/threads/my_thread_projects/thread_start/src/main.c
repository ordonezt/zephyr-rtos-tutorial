#include <zephyr.h>
#include <sys/printk.h>

//Defino un tamaño del stack del thread
#define STACKSIZE 1024 //Bytes

//Prioridad del thread
#define PRIORITY 7

#define THREAD_DELAY 1000

//Creo el stack del thread
K_THREAD_STACK_DEFINE(mi_thread_stack_area, STACKSIZE);

//Creo una estructura para guardar el contexto del thread
static struct k_thread mi_thread_data;

//Punto de entrada del thread
void mi_thread(void *arg1, void *arg2, void *arg3){
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);
    
    while(1){
        printk("mi_thread: Hola mundo!\n");

        k_msleep(THREAD_DELAY);
    }
}

void main(void){
    //Inicializo la estructura del thread
    k_thread_create(
        &mi_thread_data,                              //Puntero a estructura data del thread
        mi_thread_stack_area,                         //Puntero al stack
		K_THREAD_STACK_SIZEOF(mi_thread_stack_area),  //Tamaño del stack
		mi_thread,                                    //Punto de entrada del thread
        NULL,                                       //arg1
        NULL,                                       //arg2
        NULL,                                       //arg3
		PRIORITY,                                   //Prioridad
        0,                                          //Opciones
        K_FOREVER);                                 //Delay inicial

    //Le pongo un nombre al thread
    k_thread_name_set(&mi_thread_data, "mi_thread");

    //Le doy arranque
    k_thread_start(&mi_thread_data);
}
