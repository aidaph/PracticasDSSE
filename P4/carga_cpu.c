/*
 * practica3.c
 * 
 * Copyright 2016 Hector Perez <perezh@unican.es>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
 
#include "ev3c.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ev3c-addons.h>
#include <sched.h>


#define BELT_MOTOR_PORT  'A'         // Motor ports are named using characters
 
#define FULL_SPEED_LARGE_MOTOR   900 // units: deg/seg
#define FULL_SPEED_MEDIUM_MOTOR 1200 // units: deg/seg
 
void *motor(void *arg);
void *load(void *arg);


typedef enum stop_mode_enum
{
    COAST, BRAKE, HOLD
} stop_mode;
 
static char *STOP_MODE_STRING[] = {
    "coast","brake","hold"
};
 
typedef enum commands_enum
{
    RUN_ABS_POS, RUN_REL_POS, RUN_TIMED, STOP
} commands;
 
static char *COMMANDS_STRING[] = {
    "run-to-abs-pos","run-to-rel-pos","run-timed","stop"
};


int main (int argc, char *argv[]) {
 
  pthread_t th1, th2;
  pthread_attr_t attr;
  struct sched_param sch_param;

  // init attr
  pthread_attr_init(&attr);
  
  if(argc==0) {

    printf("No se han introducido argumentos");
    exit(1);
  }else{

        int prioridad = atoi(argv[1]);
        int carga = atoi(argv[2]);
        int duracion = atoi(argv[3]);
  	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);  
  	sch_param.sched_priority = prioridad; //sched_get_priority_max(SCHED_FIFO)-1;
  	sched_setscheduler (0, SCHED_FIFO,&sch_param );


  	//  Regular variables
  	int index            = 1;    // Loop variable
 
        printf ("*** Practica 4: LOAD CPU on EV3DEV ***\n");
   
        printf ("Voy a cargar la CPU");
        do_some_job (carga,duracion);
        printf ("Voy a dejar de cargar la CPU");


        //  Finish & close devices
        printf ("\n*** Finishing load application... OK***\n");

        //sleep(5);
        printf("thread main terminando \n");
        exit (0);
   }
}


void *load(void *arg){
    printf ("Voy a cargar la CPU");
    do_some_job (95,10);
    printf ("Voy a dejar de cargar la CPU");
}


