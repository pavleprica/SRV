
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

static void param_task(void* parametri)
{
	int* int_param = (int*)parametri;
	int i = *int_param;
	for(;;i++)
	{
		printf("Test %d\n", i);
		fflush(stdout);
		vTaskDelay(1000); //pauzira izvrsavanje taska na zadati broj milisekundi
	}
}

int main2( void )
{
	int pocetna_vrednost = 10;
	
	//Argument 4 je parametar za funkciju. Moze da bude pokazivac na bilo sta, s time
	//da funkcija onda mora pravilno da cast-uje parametar pre nego sto ga dereferencira.
	xTaskCreate(param_task, "moj task", configMINIMAL_STACK_SIZE, &pocetna_vrednost, 1, NULL);

	//Ova linija je obavezna. Ona startuje rasporedjivac, koji startuje taskove.
	//Uvek postoji makar IDLE task u sistemu, tako da ova funkcija nikada nece uraditi return.
	vTaskStartScheduler();

	//Do ove linije zapravo nikada ne dolazimo.
	for (;;);

	return 0;
}

