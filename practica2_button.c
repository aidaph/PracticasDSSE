/*
 * practica2.c
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
#include <stdio.h>
#include <ev3c.h>

#define BUTTON_NOT_AVAILABLE 	-1
#define COLOR_SENSOR_PORT		     1

typedef enum color_command_enum
{
    COL_REFLECT=0, COL_AMBIENT=1, COL_COLOR=2
} color_command;

int main () {

    ev3_sensor_ptr sensors       = NULL; //  List of available sensors
    ev3_sensor_ptr color_sensor  = NULL;

    int index, data, value;
    char *mode;


    // Init button functions
    ev3_init_button();
            
    for (index = 0; index < 15; index++) {
        value = ev3_button_pressed(BUTTON_LEFT);
        printf("Presionamos boton izquierdo? Value = %d\n", value);
        sleep(1);
    }

    //  Finish & close devices
    printf ("\n*** Finishing color sensor application... OK***\n");
    ev3_quit_button();

    return 0;
}
