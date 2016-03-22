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
#include <pthread.h>
#include <unistd.h>

#define COLOR_SENSOR_NOT_AVAILABLE 	-1
#define COLOR_SENSOR_PORT		     1
void *boton(void *arg);
void *led(void *arg);

typedef enum color_command_enum
{
    COL_REFLECT=0, COL_AMBIENT=1, COL_COLOR=2
} color_command;

int main () {

    pthread_t th1, th2;
    pthread_attr_t attr;

    ev3_sensor_ptr sensors       = NULL; //  List of available sensors
    ev3_sensor_ptr color_sensor  = NULL;

    int index, data, i, boton_pulsado;
    char *mode;

    // init attr
    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


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

   //  Create two threads
    pthread_create(&th1, &attr, boton, 0);
    pthread_create(&th2, &attr, led, &color_sensor);
    
    pthread_join(th1, NULL);
    pthread_join(th2, NULL); 
   
    //  Finish & close devices
    printf ("\n*** Finishing color sensor application... OK***\n");
    ev3_delete_sensors (sensors);
    
    sleep(30);
    printf("thread main terminando \n");
    exit (0);


}
void *boton(void *arg){
    // Init button functions
    ev3_init_button();

    int index, data, i, boton_pulsado;
    char *mode;         
    while( index < 150) {
        boton_pulsado=-1;
	for (i=BUTTON_LEFT; i<=BUTTON_BACK; i++){
                if (ev3_button_pressed(i)){
			boton_pulsado = i;
			break;	
		}
		
        }
	if (boton_pulsado != -1){
        	printf("El ultimo boton pulsado es %d\n", boton_pulsado);
        }
	usleep(300000);

    }
    ev3_quit_button();

}

void *led(void *arg){
      int index, data, i;
      char *mode;

      ev3_sensor_ptr color_sensor;
      color_sensor = *((ev3_sensor_ptr *)arg);      
      mode = color_sensor->modes[color_sensor->mode];

      ev3_init_led();
      for (index = 0; index < 150; index++) {
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
      ev3_quit_led();
}
