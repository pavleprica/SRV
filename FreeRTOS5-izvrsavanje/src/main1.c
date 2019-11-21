//
///* Standard includes. */
//#include <stdio.h>
//#include <stdlib.h>
//#include <conio.h>
//
///* FreeRTOS kernel includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//
//#define NEW_STATE 0
//#define RUNNING_STATE 1
//#define FINISHED_STATE 2
//
//struct JobType_t
//{
//	void (*func)(void*);
//	void* pvParameters;
//	TickType_t xArrival;
//	TickType_t xDeadline;
//	char cState;
//	TaskHandle_t xHandle;
//};
//
//struct BatchType_t
//{
//	struct JobType_t* pxJobs;
//	BaseType_t xJobCount;
//	int* piJobOrder;
//};
//
//void vJobWrapper(void* pvParameters)
//{
//	struct JobType_t* myJob = (struct JobType_t*)pvParameters;
//
//	myJob->cState = RUNNING_STATE;
//
//	myJob->func(myJob->pvParameters);
//
//	myJob->cState = FINISHED_STATE;
//	vTaskDelete(0);
//}
//
//void vLdfScheduler(void* pvParameters)
//{
//	struct BatchType_t* jobs = (struct BatchType_t*)pvParameters;
//	int i;
//
//	for(i=0; i < jobs->xJobCount; i++)
//	{
//		//kreiramo FreeRTOS task za job i
//		xTaskCreate(vJobWrapper, "", configMINIMAL_STACK_SIZE, jobs->pxJobs + i, 1, &(jobs->pxJobs[i].xHandle));
//	}
//
//	for(i=0; i < jobs->xJobCount; i++)
//	{
//		int currentTask = jobs->piJobOrder[i];
//
//		vTaskPrioritySet(jobs->pxJobs[currentTask].xHandle, 4);
//	}
//
//	for(; ;)
//	{
//		vTaskDelay(1);
//	}
//}
//
//void vJobCreate(struct JobType_t *job, void (*func)(void*), void* pvParameters,
//		TickType_t arrival, TickType_t deadline)
//{
//	job -> func = func;
//	job -> pvParameters = pvParameters;
//	job -> xArrival = arrival;
//	job -> xDeadline = deadline;
//	job -> cState = NEW_STATE;
//	job -> xHandle = NULL;
//}
//
//void jobWork(void* pvParameters)
//{
//	char* msg = (char*)pvParameters;
//	int i;
//
//	for(i=0; i<20; i++)
//	{
//		printf(msg);
//		fflush(stdout);
//	}
//	printf("\n");
//	fflush(stdout);
//}
//
//int* piJobOrder(struct JobType_t* jobs, int jobCount, int* piPrecedenceEdges, int edgesCount)
//{
//	int* flippedResult = (int*)malloc(jobCount * sizeof(int));
//	int* result = (int*)malloc(jobCount * sizeof(int));
//
//	int i, j;
//
//	for (i=0; i < jobCount; i++)
//	{
//		flippedResult[i] = -1;
//	}
//
//	for (i=0; i < jobCount; i++) //nalazimo novi element za result
//	{
//		int jobIsNode[jobCount]; //0 ako je posao list, 1 ako je cvor, 2 ako je vec rasporedjen
//		for (j=0; j<jobCount; j++)
//		{
//			jobIsNode[j] = 0;
//		}
//		for (j=0; j<jobCount; j++)
//		{
//			if (flippedResult[j] != -1)
//			{
//				jobIsNode[flippedResult[j]] = 2; //upisemo 2 za poslove koji su vec rasporedjeni
//			}
//		}
//
//		for (j=0; j < edgesCount; j+=2) //svaki element koji je cvor belezimo u jobIsNode
//		{
//			int jobId = piPrecedenceEdges[j] - 1;
//			if (jobId > -1)
//			{
//				jobIsNode[jobId] = 1;
//			}
//		}
//
//		int maxDeadline = 0;
//		int maxDeadlineId = -1;
//
//		for (j=0; j < jobCount; j++)
//		{
//			if (jobIsNode[j] == 0) //job is leaf
//			{
//				if (jobs[j].xDeadline > maxDeadline)
//				{
//					maxDeadline = jobs[j].xDeadline;
//					maxDeadlineId = j;
//				}
//			}
//		}
//
//		flippedResult[i] = maxDeadlineId;
//
//		//brisanje iz grafa
//		for (j=0; j < edgesCount; j+=2)
//		{
//			if (piPrecedenceEdges[j+1] == maxDeadlineId + 1)
//			{
//				piPrecedenceEdges[j] = 0;
//				piPrecedenceEdges[j+1] = 0;
//			}
//		}
//
//	}
//
//	for (i=0; i < jobCount; i++)
//	{
//		result[i] = flippedResult[jobCount-i-1];
//	}
//
//	return result;
//}
//
//int main1( void )
//{
//	//1. definisemo poslove
//	struct JobType_t job1, job2, job3, job4, job5, job6;
//	vJobCreate(&job1, jobWork, ".", 0, 20);
//	vJobCreate(&job2, jobWork, "-", 0, 50);
//	vJobCreate(&job3, jobWork, "|", 0, 40);
//	vJobCreate(&job4, jobWork, "*", 0, 30);
//	vJobCreate(&job5, jobWork, "?", 0, 50);
//	vJobCreate(&job6, jobWork, "%%", 0, 60);
//
//	struct BatchType_t batch;
//	batch.pxJobs = (struct JobType_t*)malloc(6 * sizeof(struct JobType_t));
//	batch.xJobCount = 6;
//	batch.pxJobs[0] = job1;
//	batch.pxJobs[1] = job2;
//	batch.pxJobs[2] = job3;
//	batch.pxJobs[3] = job4;
//	batch.pxJobs[4] = job5;
//	batch.pxJobs[5] = job6;
//
//	//neparni brojevi su prethodnici
//	//parni brojevi su sledbenici
//	//brojevi su logicki indeksi poslova - treba oduzeti 1 za pristup nizu
//	int piPrecedenceEdges[] = {1, 2, 1, 3, 2, 4, 2, 5, 3, 6};
//
//	int *jobOrder = piJobOrder(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10);
//	int i;
//
//	for (i=0; i < batch.xJobCount; i++)
//	{
//		printf("%d ", jobOrder[i]);
//	}
//	printf("\n");
//	fflush(stdout);
//
//	batch.piJobOrder = jobOrder;
//	//2. kreiramo nas rasporedjivac
//	xTaskCreate(vLdfScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
//
//	vTaskStartScheduler();
//
//	return 0;
//}
//
