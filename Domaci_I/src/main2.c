
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include<stdio.h>
#include<stdlib.h>

#define MAX_ARRAY_SIZE 2000000

#define TASK_COUNT 50

static int xSortedArray[MAX_ARRAY_SIZE];
static int iElementCount = 0;

int iFindPos(int val)
{
	if (iElementCount == 0)
	{
		return 0;
	}

	int left = 0;
	int right = iElementCount - 1;
	while(left <= right)
	{
		int mid = left + (right - left) / 2;

		int midVal = xSortedArray[mid];

		if (val < midVal)
		{
			right = mid-1;
		}
		else if (val > midVal)
		{
			left = mid + 1;
		}
		else
		{
			return mid;
		}

	}

	return left;
}

int iBinarySearch(int val)
{
	int pos = iFindPos(val);

	if (xSortedArray[pos] == val)
	{
		return pos;
	}
	else
	{
		return -1;
	}
}

int iInsertValue(int val)
{
	int i;
	if (iElementCount < MAX_ARRAY_SIZE)
	{
		int pos = iFindPos(val);

		if (xSortedArray[pos] != val) //ubacujemo
		{
			for (i=iElementCount-1; i >= pos; i--)
			{
				xSortedArray[i+1] = xSortedArray[i];
			}
			xSortedArray[pos] = val;
			iElementCount++;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void vPrintArray()
{
	int i;

	for (i=0; i < iElementCount; i++)
	{
		printf("%d\n", xSortedArray[i]);
		fflush(stdout);
	}

	printf("\n");
	fflush(stdout);
}

int fails[TASK_COUNT];

void vInserterTask(void* pvParameters)
{
	long i;

	int *taskId = (int*)pvParameters;

	int myFails = 0;

	for (i=0; i < MAX_ARRAY_SIZE / TASK_COUNT; i++)
	{
		int num = rand() * rand();
		int didInsert;

		didInsert = iInsertValue(num);

		if (!didInsert)
		{
			//printf("Duplicate: %d\n", num);
			//fflush(stdout);
			myFails++;
		}
	}

	fails[*taskId] = myFails;

//	vPrintArray();

	vTaskDelete(0);
}

static void vStatusTask(void* pvParameters)
{
	int i;
	int sum=0;
	for (i=0; i < TASK_COUNT; i++)
	{
		printf("%d ", fails[i]);
		sum += fails[i];
	}

	printf("| %d | %d\n", iElementCount, sum + iElementCount);

	fflush(stdout);

	vTaskDelete(0);
}

int main2( void )
{
	int taskIds[TASK_COUNT];
	int i;

	for(i=0; i<TASK_COUNT; i++)
	{
		taskIds[i] = i;

		xTaskCreate(vInserterTask, "", configMINIMAL_STACK_SIZE, taskIds + i, 2, NULL);
	}

	xTaskCreate(vStatusTask, "", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	vTaskStartScheduler();

	return 0;
}

