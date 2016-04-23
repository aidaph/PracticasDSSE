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

#define BELT_MOTOR_PORT  'A'         // Motor ports are named using characters
#define FULL_SPEED_LARGE_MOTOR   900 // units: deg/seg
#define FULL_SPEED_MEDIUM_MOTOR 1200 // units: deg/seg

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

int main () {

  //  Regular variables
  int index            = 1;    // Loop variable
  ev3_motor_ptr motors = NULL; // List of motors

  printf ("*** Practica 3: Motors on EV3DEV ***\n");
  
  motors  = ev3_load_motors(); // Loading all motors
  if (motors == NULL) {
    printf ("Error on ev3_load_motors\n");
    exit (-1);
  }
  
  // Get the target motor
  ev3_motor_ptr belt_motor = ev3_search_motor_by_port(motors, BELT_MOTOR_PORT);

  // Init motor
  ev3_reset_motor(belt_motor);
  ev3_open_motor(belt_motor);

  // Motor configuration
  ev3_stop_command_motor_by_name(belt_motor, STOP_MODE_STRING[HOLD]); 
  ev3_set_position(belt_motor, 0 );
  //ev3_set_duty_cycle_sp(belt_motor, 25);
  ev3_set_speed_sp(belt_motor, 193);
  
printf ("Start position %d (degrees)\n", ev3_get_position(belt_motor));

  //while (index < 2) { // Main loop
    // Set the time set point of a motor
    ev3_set_time_sp (belt_motor, 15000); // Running time (milliseconds)
	ev3_command_motor_by_name( belt_motor, COMMANDS_STRING[RUN_TIMED]); // Action!
    index++;
    usleep (1000);
    int count=0;
    int sum = 0;
    while ((ev3_motor_state( belt_motor) & MOTOR_RUNNING)){
    	sum+=ev3_get_speed(belt_motor);
       count++;
    }; 
    float sum_final= sum/count; // Wait until motor action is complete
    printf ("Current position %d (degrees)\n", ev3_get_position(belt_motor));
    printf("Speed= %f", sum_final);
    sleep (1); // Wait until next period
  //}
  
  //Let's reset and close the motors
  //ev3_set_position_sp(belt_motor, 360);
  //ev3_command_motor_by_name( belt_motor, COMMANDS_STRING[RUN_REL_POS]);
  usleep (1000);
  while ((ev3_motor_state( belt_motor) & MOTOR_RUNNING )); 
  sleep(5);
  printf ("Final position %d (degrees)\n", ev3_get_position(belt_motor));
  ev3_reset_motor(belt_motor);
  ev3_delete_motors(motors);
  return 0;
}

