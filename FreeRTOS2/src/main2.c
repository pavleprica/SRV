
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

static int iVal = 0;

static SemaphoreHandle_t xSem = NULL;

static void vIncrementTask(void* pvParameters)
{
	int* incNum = (int*)pvParameters;
	int i;

	for(i = 0; i < *incNum;)
	{
		if (xSemaphoreTake(xSem, (TickType_t) portMAX_DELAY) == pdTRUE)
		{
			iVal++;
			i++;

			xSemaphoreGive(xSem);
		}
	}

	vTaskDelete(0);
}

static void vDecrementTask(void* pvParameters)
{
	int* decNum = (int*)pvParameters;
	int i;
	for(i = 0; i < *decNum;)
	{
		if (xSemaphoreTake(xSem, (TickType_t) 10) == pdTRUE)
		{
			iVal--;
			i++;

			xSemaphoreGive(xSem);
		}
	}

	vTaskDelete(0);
}

static void vPrintTask(void* pvParameters)
{
	printf("Val is: %d\n", iVal);
	fflush(stdout);

	vTaskDelete(0);
}

int main2( void )
{
	int increments = 1000000;
	int decrements = 1000000;

	xTaskCreate(vIncrementTask, "inc", configMINIMAL_STACK_SIZE, &increments, 2, NULL);
	xTaskCreate(vDecrementTask, "dec", configMINIMAL_STACK_SIZE, &decrements, 2, NULL);
	xTaskCreate(vPrintTask, "print", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	xSem = xSemaphoreCreateMutex();

	if (xSem == NULL)
	{
		printf("Greska pri kreiranju muteksa. Abort\n");
		fflush(stdout);
		return 0;
	}

	vTaskStartScheduler();

	return 0;
}

