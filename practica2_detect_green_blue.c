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

    int index, data;
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
    ev3_mode_sensor (color_sensor, COL_COLOR);
    mode = color_sensor->modes[color_sensor->mode];
        
    ev3_init_led();   
    for (index = 0; index < 30; index++) {
        ev3_update_sensor_val(color_sensor);
        data = color_sensor->val_data[0].s32;
	if (data == 3 || data ==5 || data ==4){
		if (data ==3){ 
        		printf("Mode %s, Value = %d, COLOR= verde\n", mode, data);
			ev3_set_led(LEFT_LED,GREEN_LED, 255); 	
		}
		else if(data==4){
			printf("Mode %s, Value = %d, COLOR= amarillo\n", mode, data);
                        ev3_set_led(LEFT_LED,RED_LED, 150);
		}
		else{
			printf("Mode %s, Value = %d, COLOR= rojo\n", mode, data);
			ev3_set_led(LEFT_LED, RED_LED, 255);
		}
	}
	else{
		ev3_set_led(LEFT_LED,GREEN_LED, 0);
		ev3_set_led(LEFT_LED,RED_LED, 0);
	}
        sleep(1);
    }

    //  Finish & close devices
    printf ("\n*** Finishing color sensor application... OK***\n");
    ev3d_quit_led();
    ev3_delete_sensors (sensors);
    
    return 0;
}
