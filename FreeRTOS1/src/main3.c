
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

static int shared_val;

static void read(void* pvParameters)
{
	char* msg = (char*)pvParameters;
	printf("%s\n", msg);
	fflush(stdout);
	
	scanf("%d", &shared_val);
	
	//vTaskDelete(0) zaustavlja rad !trenutnog! taska. Druge moguce vrednosti za parametar u sledecem primeru.
	//Ako ce task da se zavrsi, on !mora! sebe da obrise.
	//Ako funkcija taska uradi prirodni return, to ce napraviti problem u FreeRTOS.
	//Ovo cemo detaljnije da pokrijemo kada budemo videli kako radi rasporedjivac.
	vTaskDelete(0);
}

static void write(void* pvParameters)
{
	printf("Shared val is: %d\n", shared_val);
	fflush(stdout);
	
	vTaskDelete(0);
}

int main3( void )
{
	//Prioriteti su 2 i 1, tj. prvo se izvrsava citanje, pa onda pisanje.
	xTaskCreate(read, "read", configMINIMAL_STACK_SIZE, "Input a number, please", 2, NULL);
	xTaskCreate(write, "write", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	//Do ove linije zapravo nikada ne dolazimo.
	for (;;);

	return 0;
}

