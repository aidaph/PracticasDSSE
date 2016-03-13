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

#define COLOR_SENSOR_NOT_AVAILABLE 	-1
#define COLOR_SENSOR_PORT		     1

typedef enum color_command_enum
{
    COL_REFLECT=0, COL_AMBIENT=1, COL_COLOR=2
} color_command;

int main () {

    ev3_sensor_ptr sensors       = NULL; //  List of available sensors
    ev3_sensor_ptr color_sensor  = NULL;

    int index, data, i, boton_pulsado;
    char *mode;

    // Loading all sensors
    sensors = ev3_load_sensors();
    if (sensors == NULL)
        return COLOR_SENSOR_NOT_AVAILABLE;

    // Get color sensor by port
    color_sensor = ev3_search_sensor_by_port( sensors, COLOR_SENSOR_PORT);
    if (color_sensor == NULL)
        return COLOR_SENSOR_NOT_AVAILABLE;

    // Init sensor
    color_sensor = ev3_open_sensor(color_sensor);
    if (color_sensor == NULL)
        return COLOR_SENSOR_NOT_AVAILABLE;   

    // Set mode
    ev3_mode_sensor (color_sensor, COL_AMBIENT);
    mode = color_sensor->modes[color_sensor->mode];
    
    // Init button functions
    ev3_init_button();
    
       
    while( index < 150) {
        ev3_update_sensor_val(color_sensor);
	for (i=BUTTON_LEFT; i<=BUTTON_BACK; i++){
                if (ev3_button_pressed(i)){
			boton_pulsado = i;
			break;	
		}
		
        }
	
        data = color_sensor->val_data[0].s32;
        printf("Mode %s, Value = %d\n", mode, data);
        printf("El ultimo boton pulsado es %d\n", boton_pulsado);
        
	usleep(100000);

    }

    //  Finish & close devices
    printf ("\n*** Finishing color sensor application... OK***\n");
    ev3_delete_sensors (sensors);
    // Finishes the work with the buttons
    ev3_quit_button();
    return 0;
}
