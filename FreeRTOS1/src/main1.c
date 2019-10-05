
/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

//funkcije taskova cemo tipicno pisati kao static, tj. "private"
static void jednostavan_task(void* parametri)
{
	int i=0;
	for(;;i++)
	{
		printf("Test %d\n", i);
		fflush(stdout); //eclipse uglavnom zahteva ovo da bi se tekst zapravo prikazao na ekranu
		vTaskDelay(1000); //pauzira izvrsavanje taska na zadati broj milisekundi
	}
}

int main1( void )
{
	//Kreira task. Argumenti su, redom:
	//1. Funkcija taska
	//2. Stringovni naziv taska - nema funkciju u samom OS
	//3. Velicina steka
	//4. Parametri (tip je void*)
	//5. Prioritet - 0 je najmanje bitna, configMAX_PRIORITIES, koji je 7, je najbitnije
	//6. Handle - prosledjuje se po referenci i koristi se za rad sa taskom u buducnosti
	xTaskCreate(jednostavan_task, "moj task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	//Ova linija je obavezna. Ona startuje rasporedjivac, koji startuje taskove.
	//Uvek postoji makar IDLE task u sistemu, tako da ova funkcija nikada nece uraditi return.
	//IDLE task ima minimalni prioritet - 0. Dok god nas task ima prioritet veci od toga, IDLE nece da dodje na red
	vTaskStartScheduler();

	//Do ove linije zapravo nikada ne dolazimo.
	for (;;);

	return 0; //sluzi da ukloni eclipse warning za int funkciju koja ne vraca int vrednost
}

