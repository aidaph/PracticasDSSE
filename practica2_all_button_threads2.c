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
void *lcd(void *arg);

typedef enum color_command_enum
{
    COL_REFLECT=0, COL_AMBIENT=1, COL_COLOR=2
} color_command;

struct shared_data {
  pthread_mutex_t mutex;
  int color_sensor, boton_pulsado;
  char *mode;

};


int main () {

    // Define mutex attr, threads, threads attr and the shared data struct
    pthread_mutexattr_t mutexattr;
    pthread_t th1, th2, th3;
    pthread_attr_t attr;
    struct shared_data data;
    int ret;

    
    ev3_sensor_ptr sensors       = NULL; //  List of available sensors
    ev3_sensor_ptr color_sensor  = NULL;

    int index, i;
    char *mode;
   
    pthread_mutexattr_init (&mutexattr);
    if ((ret = pthread_mutex_init (&data.mutex,&mutexattr)))
    printf ("error in mutex init:%s\n", strerror (ret));

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

    // Assing value to shared data
    data.color_sensor = color_sensor;
    data.mode = mode;

    //  Create two threads
    pthread_create(&th1, &attr, boton, &data);
    //pthread_create(&th2, &attr, led, &data);
    //pthread_create(&th3, &attr, lcd, &data);

    pthread_join(th1, NULL);
    //pthread_join(th2, NULL); 
    //pthread_join(th3, NULL);
    //  Finish & close devices
    printf ("\n*** Finishing color sensor application... OK***\n");
    ev3_delete_sensors (sensors);
    
    sleep(30);
    printf("thread main terminando \n");
    exit (0);


}
void *lcd(void *arg){

    ev3_init_lcd();

    struct shared_data *data = (struct shared_data *)arg;


    pthread_mutex_lock (&data->mutex);
    ev3_text_lcd_normal(8,20,data->boton_pulsado);
    pthread_mutex_unlock (&data->mutex);
    //ev3_text_lcd_normal(8,20,data->color_sensor);

    //ev3_text_lcd_normal(8,20,data->mode);

    ev3_clear_lcd();
    pthread_exit ((void *)0);
    return NULL; // never reached, just to avoid warning

}
void *boton(void *arg){
    // Init button functions
    ev3_init_button();

    int index, boton_pulsado, i;
    struct shared_data *data = (struct shared_data *)arg;
    //char *mode;         
    while( index < 150) {
        boton_pulsado=-1;
	for (i=BUTTON_LEFT; i<=BUTTON_BACK; i++){
                if (ev3_button_pressed(i)){
			boton_pulsado = i;

                        // Lock mutex to share the boton_pulsado variable
    			pthread_mutex_lock (&data->mutex);
                        //data->boton_pulsado = boton_pulsado;

    			pthread_mutex_unlock (&data->mutex);
			break;	
		}
		
        }
	if (boton_pulsado != -1){
		char string[80];
		sprintf("El ultimo boton pulsado es %d", boton_pulsado);
        }
	usleep(300000);

    }
    ev3_quit_button();
    pthread_exit ((void *)0);
    return NULL; // never reached, just to avoid warning
}

void *led(void *arg){
      int index, data, i;
      char *mode;


      struct shared_data *data2 = (struct shared_data *)arg;
      ev3_sensor_ptr color_sensor;
      color_sensor = *((ev3_sensor_ptr *)arg);      
      mode = color_sensor->modes[color_sensor->mode];

      ev3_init_lcd();

      ev3_init_led();
      for (index = 0; index < 150; index++) {
        ev3_update_sensor_val(color_sensor);
        data = color_sensor->val_data[0].s32;
        if (data == 3 || data ==5 || data ==4){
		
                if (data == 3){
		        pthread_mutex_lock (&data2->mutex);
                        data2->mode = mode;
		        data2->color_sensor = color_sensor;
                        pthread_mutex_unlock (&data2->mutex);	

			printf("Mode %s, Value = %d\n", mode, data);
                        ev3_set_led(LEFT_LED,GREEN_LED, 255);
                }
                else if(data==4){
			pthread_mutex_lock (&data2->mutex);
                        data2->mode = mode;
                        data2->color_sensor = color_sensor;

                        pthread_mutex_unlock (&data2->mutex);

                        ev3_set_led(LEFT_LED,RED_LED, 150);
                }
                else{
			pthread_mutex_lock (&data2->mutex);
                        data2->mode = mode;
                        data2->color_sensor = color_sensor;

                        pthread_mutex_unlock (&data2->mutex);
                        ev3_set_led(LEFT_LED, RED_LED, 255);
		}
	}
        else{
		pthread_mutex_lock (&data2->mutex);
                data2->mode = mode;
                data2->color_sensor = color_sensor;
                pthread_mutex_unlock (&data2->mutex);

                printf("Mode %s, Value = %d\n", mode, data);
                
                ev3_set_led(LEFT_LED,GREEN_LED, 0);
                ev3_set_led(LEFT_LED,RED_LED, 0);
        }
        sleep(1);
      }
      ev3_quit_led();
      ev3_clear_lcd();
      pthread_exit ((void *)0);
      return NULL; // never reached, just to avoid warning
}
