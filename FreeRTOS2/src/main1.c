
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

static int iVal = 0;

static void vIncrementTask(void* pvParameters)
{
	int* incNum = (int*)pvParameters;
	int i;
	for(i = 0; i < *incNum; i++)
	{
		iVal++;
	}

	vTaskDelete(0);
}

static void vDecrementTask(void* pvParameters)
{
	int* decNum = (int*)pvParameters;
	int i;
	for(i = 0; i < *decNum; i++)
	{
		iVal--;
	}

	vTaskDelete(0);
}

static void vPrintTask(void* pvParameters)
{
	printf("Val is: %d\n", iVal);
	fflush(stdout);

	vTaskDelete(0);
}

int main1( void )
{
	int increments = 100000000;
	int decrements = 100000000;

	xTaskCreate(vIncrementTask, "inc", configMINIMAL_STACK_SIZE, &increments, 2, NULL);
	xTaskCreate(vDecrementTask, "dec", configMINIMAL_STACK_SIZE, &decrements, 2, NULL);
	xTaskCreate(vPrintTask, "print", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	//Ova linija je obavezna. Ona startuje rasporedjivac, koji startuje taskove.
	//Uvek postoji makar IDLE task u sistemu, tako da ova funkcija nikada nece uraditi return.
	//IDLE task ima minimalni prioritet - 0. Dok god nas task ima prioritet veci od toga, IDLE nece da dodje na red
	vTaskStartScheduler();

	return 0; //sluzi da ukloni eclipse warning za int funkciju koja ne vraca int vrednost
}

