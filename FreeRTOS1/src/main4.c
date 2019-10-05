
/*
 * Postoje dve vrste taskova: control i print.
 *
 * U sistemu je uvek aktivan jedan control task i 0 do 10 print taskova.
 *
 * Control task je zaduzen za startovanje i zaustavljanje print taskova,
 * i on nudi korisniku komande da to ucini.
 *
 * Print taskovi stampaju svoj redni broj i prave pauzu od isto toliko sekundi.
 */

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#define START 1
#define STOP 2

#define MAX_TASKS 10

//definisemo handle i parametre kao globalne promenljive
//da bismo izbegli probleme sa vidljivoscu kasnije
static xTaskHandle print_tasks[MAX_TASKS];
static int print_task_params[MAX_TASKS];

static void print_num(void* pvParameters)
{
	//parametar mi je redni broj - to pisem na ekranu, i toliko sekundi se blokiram izmedju pisanja
	int* num_param = (int*)pvParameters;

	int num = *num_param;
	for(;;)
	{
		printf("%d\n", num);
		fflush(stdout);

		vTaskDelay(1000 * num);
	}

	//Nikada necemo doci dovde. Prekida nas control task.
	vTaskDelete(0);
}

static void control(void* pvParameters)
{
	for(;;)
	{
		int option;
		printf("%d. Start\n%d. Stop\n", START, STOP);
		fflush(stdout);

		scanf("%d", &option);

		if (option == START)
		{
			int which;
			printf("Which task are we starting? (0-9]\n");
			fflush(stdout);

			scanf("%d", &which);

			if (print_tasks[which] != NULL)
			{
				printf("That task is already started.\n");
				fflush(stdout);
			}
			else
			{
				print_task_params[which] = which;

				//parametri se nalaze u globalnom nizu, sto je veoma bitno
				//mogli bismo da probamo da prosledimo parametar u funkciju kao &which, bez niza
				//ali problem je sto je promenljiva which definisana na nivou ovog if bloka
				//sto znaci da kada funkcija taska pokusa da je procita, ona mozda vise nece postojati

				//prioritet je 2, jer hocemo da print task uleti preko control taska, da ispise svoje,
				//nakon cega ce se blokirati, tako da control task moze da nastavi sa radom na konzoli.

				//primetite poslednji argument funkcije - handle, prosledjen po referenci
				//u delu control taska koji zaustavlja taskove, taj handle se koristi da se
				//naznaci koji task hocemo da zaustavimo
				xTaskCreate(print_num, "", configMINIMAL_STACK_SIZE, print_task_params + which, 2, &(print_tasks[which]));
			}
		}
		else if (option == STOP)
		{
			int which;
			printf("Which task are we stopping? (0-9]\n");
			fflush(stdout);

			scanf("%d", &which);

			if (print_tasks[which] != NULL)
			{
				//vTaskDelete sa argumentom zaustavlja rad taska ciji handle smo prosledili
				vTaskDelete(print_tasks[which]);

				print_tasks[which] = NULL;
				print_task_params[which] = -1;
			}
			else
			{
				printf("That task is not started.\n");
				fflush(stdout);
			}
		}
		else
		{
			printf("Wrong option.\n");
			fflush(stdout);
		}
	}
	
	vTaskDelete(0);
}

int main4( void )
{
	int i;
	for (i=0; i<MAX_TASKS; i++)
	{
		print_tasks[i] = NULL;
		print_task_params[i] = -1;
	}

	//control task ima prioritet 1, dok ce print taskovi imati prioritet 2
	xTaskCreate(control, "control", configMINIMAL_STACK_SIZE, "Input a number, please", 1, NULL);

	vTaskStartScheduler();

	//Do ove linije zapravo nikada ne dolazimo.
	for (;;);

	return 0;
}

