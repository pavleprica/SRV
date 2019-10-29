
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define QUEUE_SIZE 10

static QueueHandle_t xMyQueue;

static int iQueueEmpty()
{
	return uxQueueMessagesWaiting(xMyQueue) == 0;
}

static int iQueueFull()
{
	return uxQueueMessagesWaiting(xMyQueue) == QUEUE_SIZE;
}

static void vQueuePut(int val)
{
	xQueueSend(xMyQueue, &val, (TickType_t) portMAX_DELAY);
}

static int iQueueTake()
{
	int iToReturn = -1;
	xQueueReceive(xMyQueue, &iToReturn, portMAX_DELAY);

	return iToReturn;
}

/*
 * Task koji vrsi pisanje. Parametar je pocetna brojka. Pise se 100 vrednosti pocevsi od te brojke.
 *
 * Nas queue je lepo implementiran ako ovde nema traga od sinhronizacije.
 */
static void vWriterTask(void* pvParameters)
{
	int* startVal = (int*)pvParameters;
	int i;

	for (i=*startVal; i<(*startVal)+100;)
	{
		if (!iQueueFull()) 					//ako ima mesta
		{
			vQueuePut(i); 					//pisi - ovo je bezbedna operacija
			printf("Wrote val: %d\n", i);
			fflush(stdout);

			//vTaskDelay((TickType_t) 500); 	//test 1 - sporo pisanje
			vTaskDelay((TickType_t) 100); //test 2 - brzo pisanje
			i++;
		}
	}

	vTaskDelete(0);
}

static void vReaderTask(void* pvParameters)
{
	for (;;)
	{
		if (!iQueueEmpty())					//ako ima mesta
		{
			int val = iQueueTake();			//citaj - ovo je bezbedna operacija
			printf("Read val: %d\n", val);
			fflush(stdout);
			//vTaskDelay(100); 				//test 1 - brzo citanje
			vTaskDelay(500); 				//test 2 - sporo citanje
		}
	}
}

int main1( void )
{
	xMyQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));

	int writerParam1 = 0;
	int writerParam2 = 1000;
	int writerParam3 = 2000;

	xTaskCreate(vWriterTask, "", configMINIMAL_STACK_SIZE, &writerParam1, 1, NULL);
	xTaskCreate(vWriterTask, "", configMINIMAL_STACK_SIZE, &writerParam2, 1, NULL);
	xTaskCreate(vWriterTask, "", configMINIMAL_STACK_SIZE, &writerParam3, 1, NULL);

	xTaskCreate(vReaderTask, "", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	return 0;
}

