/*
 * vezba_alarmi.c
 *
 *  Created on: 05.10.2019.
 *      Author: pavle
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

//Number of alarms
#define ALARM_NUMBER 10

#define NAME_AND_MESSAGE_LEN 20

//Options
#define NEW_ALARM 1
#define PRINT_ALL_ALARMS 2
#define MODIFY_ALARM 3
#define DELETE_ALARM 4
#define EXIT 0

//Alarm parameters
static char alarm_names[ALARM_NUMBER][NAME_AND_MESSAGE_LEN];
static int alarm_time[ALARM_NUMBER];
static char alarm_message[ALARM_NUMBER][NAME_AND_MESSAGE_LEN];

static int alarm_indexes[ALARM_NUMBER];

//AlarmTaskovi
static xTaskHandle alarm_tasks[ALARM_NUMBER];

static int control_status = 1;

static void remove_alarm(int num) {

    if(num == -1) {

        int i, j;
        for(i = 0; i < ALARM_NUMBER; i++) {
            for(j = 0; j < NAME_AND_MESSAGE_LEN; j++) {
                alarm_names[i][j] = 0;
                alarm_message[i][j] = 0;
            }
        }
        for(i = 0; i < ALARM_NUMBER; i++) {
            alarm_time[i] = -1;
            alarm_indexes[i] = -1;
            alarm_tasks[i] = NULL;
        }

    } else {

        if(alarm_tasks[num] != NULL) {

            vTaskDelete(alarm_tasks[num]);

            alarm_tasks[num] = NULL;

            int i;
            for(i = 0; i < NAME_AND_MESSAGE_LEN; i++) {
                alarm_names[num][i] = 0;
                alarm_message[num][i] = 0;
            }
            alarm_time[num] = -1;
            alarm_indexes[num] = -1;
        }

    }
}

static void alarm(void *param) {
    int *task_param = (int*)param;

    int task_index = *task_param;

    while(1) {
        vTaskDelay(1000);
        alarm_time[task_index]--;

        if(alarm_time[task_index] <= 0) {
            printf("%s\n", alarm_message[task_index]);
            fflush(stdout);
            remove_alarm(task_index);
            break;
        }
    }

    vTaskDelete(0);
}

static void create_new_alarm() {

    int i;
    int free_index = -1;

    for(i = 0; i < ALARM_NUMBER; i++) {
        if(alarm_tasks[i] == NULL) {
            free_index = i;
            break;
        }
    }

    if(free_index == -1) {
        printf("Nema mesta za alarm :(\n");
        fflush(stdout);
        return;
    }

    char name[NAME_AND_MESSAGE_LEN], message[NAME_AND_MESSAGE_LEN];
    int time;

    printf("Unesite ime alarma: \n");
    fflush(stdout);
    scanf("%s", &name);
    strcpy(alarm_names[free_index], name);

    printf("Unesite poruku alarma: \n");
    fflush(stdout);
    scanf("%s", &message);
    strcpy(alarm_message[free_index], message);

    printf("Unesite vreme: \n");
    fflush(stdout);
    scanf("%d", &time);
    alarm_time[free_index] = time;

    alarm_indexes[free_index] = free_index;

    xTaskCreate(alarm, "alarm", configMINIMAL_STACK_SIZE, alarm_indexes + free_index, 2, &(alarm_tasks[free_index]));
}

static void print_all_alarms() {
    int i;

    for(i = 0; i < ALARM_NUMBER; i++) {
        if(alarm_tasks[i] != NULL) {
            printf("ALARM [%d]\n", i);
            printf("Alarm name: %s\n", alarm_names[i]);
            printf("Alarm message: %s\n", alarm_message[i]);
            fflush(stdout);
        }
    }
}

static int find_index_of_alarm(char *name) {

    int i = 0;
    for(i = 0; i < ALARM_NUMBER; i++) {
        if(strcmp(name, alarm_names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

static void modify_alarm() {

    char name[NAME_AND_MESSAGE_LEN];
    printf("Alarm name: \n");
    fflush(stdout);
    scanf("%s", &name);

    int alarm_index = find_index_of_alarm(name);

    if(alarm_index == -1) {
        printf("Ne postoji alarm sa tim imenom.\n");
        fflush(stdout);
        return;
    }

    char message[NAME_AND_MESSAGE_LEN];
    printf("Unesite poruku: \n");
    fflush(stdout);
    scanf("%s", &message);
    strcpy(alarm_message[alarm_index], message);

    int time;
    printf("Unesite vreme: \n");
    fflush(stdout);
    scanf("%d", &time);
    alarm_time[alarm_index] = time;
}


static void delete_alarm() {
    char name[NAME_AND_MESSAGE_LEN];
    printf("Alarm name: \n");
    fflush(stdout);
    scanf("%s", &name);

    int alarm_index = find_index_of_alarm(name);

    if(alarm_index == -1) {
        printf("Ne postoji alarm sa tim imenom.\n");
        fflush(stdout);
        return;
    }

    remove_alarm(alarm_index);
}


static void control_task(void *param) {

    while(control_status) {
        int option;
        printf("Choose your option:\n");
        printf("[%d] -> Novi alarm\n", NEW_ALARM);
        printf("[%d] -> Stampanje svih alarma\n", PRINT_ALL_ALARMS);
        printf("[%d] -> Izmeni alarm\n", MODIFY_ALARM);
        printf("[%d] -> Izbrisi alarm\n", DELETE_ALARM);
        printf("[%d] -> Izlaz\n", EXIT);
        printf("Opcija: \n");
        fflush(stdout);

        scanf("%d", &option);

        switch (option) {
            case EXIT:
                control_status = 0;
                break;
            case NEW_ALARM:
                create_new_alarm();
                break;
            case PRINT_ALL_ALARMS:
                print_all_alarms();
                break;
            case MODIFY_ALARM:
                modify_alarm();
                break;
            case DELETE_ALARM:
                delete_alarm();
                break;
        }
    }

    vTaskDelete(0);

}








int vezba_alarmi(void) {

    remove_alarm(-1);

    xTaskCreate(control_task, "control task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();


    return 0;
}
