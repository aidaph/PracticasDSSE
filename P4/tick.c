/*
 * tick.c
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
 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <addons/ev3c-addons.h>
#include <misc/timespec_operations.h>

#define MEASURES        100
#define MAX_LOOPS       10000000
#define THRESHOLD_NS    60000   // 60 us; adjust as needed

// Thread that checks the system tick
void * high_prio_thread (void *arg)
{
    struct timespec initial_time,last_time, now, interval;
    struct timespec list_interval[MEASURES];
    struct timespec list_time[MEASURES];
    struct timespec threshold= {0,THRESHOLD_NS}; 
    int num=0;
    long i=0;

    // read the initial time
    clock_gettime(CLOCK_MONOTONIC, &initial_time);

    // read the time
    clock_gettime(CLOCK_MONOTONIC, &last_time);
    while(i<MAX_LOOPS && num<MEASURES) {
        // read the time
        clock_gettime(CLOCK_MONOTONIC, &now);
        // calculate difference
        interval=now;
        decr_timespec(&interval,&last_time);
        // if difference is above threshold, store in list
        if (smaller_timespec(&threshold,&interval)) {
            list_interval[num]=interval;
            list_time[num]=now;
            num++;
            clock_gettime(CLOCK_MONOTONIC, &now);
        }
        last_time=now;
        i++;
    }
    for (i=1; i<num; i++) { // Discard first measure
        decr_timespec(&(list_time[i]),&initial_time); // Make it relative to the initial time
        printf("Measure over threshold %g us at time %g ms\n",
               t2d (list_interval[i])*1E6, t2d (list_time[i])*1E3); // timespec to double conversion
    }
}

// Low priority thread that tries using the CPU
void * low_prio_thread (void *arg)
{
    do_some_job (100,10); // Max load on CPU for 10 secs
}

// Main program that creates two threads
int main ()
{
    pthread_t t1,t2;
    struct sched_param sch_param;
    pthread_attr_t attr;

    // Set the priority of the main program to max_prio
    sch_param.sched_priority = (sched_get_priority_max(SCHED_FIFO));
    if (pthread_setschedparam(pthread_self(),SCHED_FIFO,&sch_param) !=0)
    {
        printf("Error while setting main thread's priority\n");
        exit(1);
    }

    // Create the thread attributes object
    if (pthread_attr_init (&attr) != 0)
    {
        printf("Error while initializing attributes\n");
        exit(1);
    }

    // Set thread attributes

    // Never forget the inheritsched attribute
    // Otherwise the scheduling attributes are not used
    if (pthread_attr_setinheritsched (&attr,PTHREAD_EXPLICIT_SCHED) != 0)
    {
        printf("Error in inheritsched attribute\n");
        exit(1);
    }

    // Thread is created joinable
    if (pthread_attr_setdetachstate (&attr,PTHREAD_CREATE_JOINABLE) != 0)
    {
        printf("Error in detachstate attribute\n");
        exit(1);
    }

    // The scheduling policy is fixed-priorities, with
    // FIFO ordering for threads of the same priority
    if (pthread_attr_setschedpolicy (&attr, SCHED_FIFO) != 0)
    {
        printf("Error in schedpolicy attribute\n");
        exit(1);
    }

    // Set the priority of thread 2 to min_prio+50
    sch_param.sched_priority = (sched_get_priority_min(SCHED_FIFO)+50);
    if (pthread_attr_setschedparam (&attr,&sch_param) != 0)
    {
        printf("Error in schedparam attribute\n");
        exit(1);
    }

    // create thread 2 with the attributes specified in attr
    if (pthread_create (&t2,&attr,low_prio_thread,NULL) != 0) {
        printf("Error when creating thread 2\n");
    }

    // Let thread 2 start the execution
    sleep (1.0);

    // Set the priority of thread 1 to max_prio
    // the other attributes remain the same for both threads
    sch_param.sched_priority = (sched_get_priority_max(SCHED_FIFO));
    if (pthread_attr_setschedparam (&attr,&sch_param) != 0)
    {
        printf("Error in schedparam attribute\n");
        exit(1);
    }

    // create thread 1 with the attributes specified in attr
    if (pthread_create (&t1,&attr,high_prio_thread,NULL) != 0) {
        printf("Error when creating thread 1\n");
    }

    // wait until high prio thread finishes
    pthread_join(t1,NULL);
    exit(0);
}
