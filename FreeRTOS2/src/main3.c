
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*
 * Nas red ce biti velicine 10, s time da cemo u njega smestati najvise 9 stavki u jednom trenutku.
 */
#define QUEUE_SIZE 10

/*
 * x je tipican prefiks za nizove, strukture i sl.
 */
static int xMyQueue[QUEUE_SIZE];
static int iWritePtr = 0; //na ovo mesto ce put funkcija da pise, i da nakon toga pomeri pokazivac unapred
static int iReadPtr = 0; //sa ovog mesta ce take funkcija da cita, i da nakon toga pomeri pokazivac unapred

static xSemaphoreHandle xWriteMutex; //imacemo vise writer taskova - ovaj mutex ce da sluzi za njihovo medjusobno iskljucivanje
static xSemaphoreHandle xQueueEmptySem; //signalizira da je red prazan
static xSemaphoreHandle xQueueFullSem; //signalizira da je red pun

/*
 * Uzimamo konvenciju gde je red prazan ako pokazivaci imaju istu vrednost (npr.: write=0, read=0)
 */
static int iQueueEmpty()
{
	return iWritePtr == iReadPtr;
}

/*
 * Uzimamo konvenciju da je red pun ako write pokazuje na jedno mesto pre read (npr.: write=9, read=0)
 */
static int iQueueFull()
{
	return ((iWritePtr + 1) % QUEUE_SIZE) == iReadPtr;
}

/*
 * Funkcija za smestanje stavke u red. Radi sledece:
 * 1) Blokira se u slucaju da je red pun pomocu "full" semafora.
 * 2) Nakon sto zna da red nije pun, uzme "write" mutex da bi bi trenutni task bio iskljucivi pisac.
 * 3.1) Pise u red i menja vrednost "write" pokazivaca.
 * 3.2) Signalizira da je nesto stavljeno u red, tako da neki reader mozda zeli da se probudi.
 * 3.3) Oslobodi "write" mutex.
 */
static void vQueuePut(int val)
{
	if (xSemaphoreTake(xQueueFullSem, (TickType_t) portMAX_DELAY) == pdTRUE) //1
	{
		if (xSemaphoreTake(xWriteMutex, (TickType_t) portMAX_DELAY) == pdTRUE) //2
		{
			xMyQueue[iWritePtr] = val; //3.1
			iWritePtr = (iWritePtr + 1) % QUEUE_SIZE;

			xSemaphoreGive(xQueueEmptySem); //3.2

			xSemaphoreGive(xWriteMutex); //3.3
		}
	}
}

/*
 * Posto znamo da cemo da imamo samo jedan task za citanje, ova funkcija je jednostavnija od write.
 *
 * 1) Blokira se u slucaju da je red prazan, pomocu "empty" semafora.
 * 2.1) Procita vrednost iz reda i menja vrednost "read" pokazivaca.
 * 2.2) Signalizira da je nesto procitano iz reda, tako da neki pisac moze da se probudi.
 *
 * VEZBA: prepraviti ovu funkciju tako da radi za veci broj citalaca.
 */
static int iQueueTake()
{
	int to_return = -1;
	if (xSemaphoreTake(xQueueEmptySem, (TickType_t)portMAX_DELAY) == pdTRUE) //1
	{
		to_return = xMyQueue[iReadPtr]; //2.1
		iReadPtr = (iReadPtr + 1) % QUEUE_SIZE;

		xSemaphoreGive(xQueueFullSem); //2.2
	}

	return to_return;
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

			vTaskDelay((TickType_t) 500); 	//test 1 - sporo pisanje
			//vTaskDelay((TickType_t) 100); //test 2 - brzo pisanje
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
			vTaskDelay(100); 				//test 1 - brzo citanje
			//vTaskDelay(500); 				//test 2 - sporo citanje
		}
	}
}

int main3( void )
{
	xWriteMutex = xSemaphoreCreateMutex();
	xQueueEmptySem = xSemaphoreCreateCounting(QUEUE_SIZE, 0);
	xQueueFullSem = xSemaphoreCreateCounting(QUEUE_SIZE, QUEUE_SIZE-1);

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

