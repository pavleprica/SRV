//
// Created by pavle on 12.11.2019..
//

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#define NEW_STATE 0
#define RUNNING_STATE 1
#define FINISHED_STATE 2

int piPrecedenceEdges[] = {1, 2, 1, 3, 2, 4, 2, 5, 3, 6};
double weight = 10;

struct JobType_t
{
    void (*func)(void*);
    void* pvParameters;
    int job_id;
    TickType_t xArrival;
    TickType_t xDeadline;
    TickType_t xC;
    struct Resource *resources;
    int resource_count;
    char cState;
    TaskHandle_t xHandle;
};

struct Resource {
    TickType_t timeOfStartAvailability;
};

struct BatchType_t
{
    struct JobType_t* pxJobs;
    BaseType_t xJobCount;
    int* piJobOrder;
};

struct BatchType_t batch;

void load_file();

void vJobWrapper(void* pvParameters);

void vScheduler(void* pvParameters);

void vJobCreate(struct JobType_t *job, int job_id, void (*func)(void*), void* pvParameters,
                TickType_t arrival, TickType_t deadline, TickType_t C);

void vJobAssingResource(struct JobType_t *job, struct Resource *resources, int resource_count);

void vResourceCreate(struct Resource *resource, TickType_t timeAvailability);

void jobWork(void* pvParameters);

struct JobType_t* JobOrder_FCFS(struct JobType_t* jobs, int jobCount);

struct JobType_t* JobOrder_SJF(struct JobType_t* jobs, int jobCount);

struct JobType_t* JobOrder_EDF(struct JobType_t* jobs, int jobCount);

struct JobType_t* JobOrder_EDF_W_SJF(struct JobType_t* jobs, int jobCount, double weight);

struct JobType_t* JobOrder_Test(struct JobType_t* jobs, int jobCount);

struct JobType_t* JobOrder_EDF_W_Test(struct JobType_t* jobs, int jobCount, double weight);

int check_if_precedence(struct JobType_t* jobs, int jobCount,  int* piPrecedenceEdges, int edgesCount);
int check_if_doable(struct JobType_t* jobs, int jobCount);

void flush_all();

int domaci_II() {
    load_file();


    return 0;
}


void load_file() {
    char *heruistik = (char*) malloc (50 * sizeof(char));
    char job_id[20], job_arrival[20], job_time[20], job_deadline[20], print_text[20][20];
    char *res = (char*) malloc (15 * sizeof(char));

    FILE *file;
    fflush(stdin);
    file = fopen("D:\\putanja\\data.txt", "r");

    fscanf(file, "%[^\n]", heruistik);
    fgetc(file);

    int job_counter = 0;

    while(fscanf(file, "%[^;]%*c%[^;]%*c%[^;]%*c%[^;]%*c%[^;]%*c%[^\n]",
                 job_id,
                 job_arrival,
                 job_time,
                 job_deadline,
                 print_text[job_counter], res) != EOF) {
        job_counter++;

        fgetc(file);
    }
    fclose(file);

    fflush(stdin);
    file = fopen("D:\\putanja\\data.txt", "r");

    fscanf(file, "%[^\n]", heruistik);
    fgetc(file);

    batch.pxJobs = (struct JobType_t*)malloc(job_counter * sizeof(struct JobType_t));
    batch.xJobCount = job_counter;

    struct JobType_t *jobs = (struct JobType_t*) malloc (job_counter * sizeof(struct JobType_t));

    int i = 0;
    while(fscanf(file, "%[^;]%*c%[^;]%*c%[^;]%*c%[^;]%*c%[^;]%*c%[^\n]",
                 job_id,
                 job_arrival,
                 job_time,
                 job_deadline,
                 print_text[i], res) != EOF) {
        int job_i = atoi(job_id), job_arr = atoi(job_arrival), job_t = atoi(job_time), job_d = atoi(job_deadline);

        vJobCreate(jobs + i, job_i, jobWork, print_text[i], job_arr, job_d, job_t);

        char *res_tmp = (char*) malloc (15 * sizeof(char));
        res[strlen(res) - 1] = 0;
        strcpy(res_tmp, res);
        char splited_res[20];
        int res_counter = 0;
        int len_counter = 0;
        int res_len = strlen(res);
        res++;
        res_tmp++;

        do {
        	sscanf(res, "%[^,]", splited_res);
        	res_counter++;

        	len_counter += strlen(splited_res) + 2;
        	res += strlen(splited_res) + 2;
        } while(len_counter <= res_len);

        len_counter = 0;
        res_len = strlen(res_tmp);

        struct Resource *resources = (struct Resource*) malloc(res_counter * sizeof(struct Resource));
        int c = 0;
        do {
            sscanf(res_tmp, "%[^,]", splited_res);
            int z = 2;
            int num = 0;
            while(splited_res[z] != ']') {
                num = num * 10 + splited_res[z] - '0';
                z++;
            }
            resources[c].timeOfStartAvailability = num;
            c++;
            len_counter += strlen(splited_res) + 2;
            res_tmp += strlen(splited_res) + 2;
        } while(len_counter <= res_len);

        vJobAssingResource(jobs + i, resources, res_counter);

        i++;
        fgetc(file);
    }
    flush_all();
    fclose(file);

    for(i = 0; i < batch.xJobCount; i++) {
       batch.pxJobs[i] = jobs[i];
    }


    char heruistik_tmp[50];
    int heruistik_len = strlen(heruistik);
    *(heruistik + heruistik_len) = '|';
    *(heruistik + heruistik_len + 1) = 0;
    heruistik_len += 1;
    int heruistik_pos = 0;
    while(heruistik_pos < heruistik_len) {
        sscanf(heruistik, "%[^|]", heruistik_tmp);
        heruistik_pos += strlen(heruistik_tmp) + 1;
        heruistik += 2;

        int flag_doable = 1;
        if(strcmp(heruistik_tmp, "a") == 0) {
            batch.pxJobs = JobOrder_FCFS(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "C") == 0) {
            batch.pxJobs = JobOrder_SJF(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "d") == 0) {
            batch.pxJobs = JobOrder_EDF(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "d + W * C") == 0) {
            batch.pxJobs = JobOrder_EDF_W_SJF(batch.pxJobs, batch.xJobCount, weight);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "Test") == 0) {
            batch.pxJobs = JobOrder_Test(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "d + W * Test") == 0) {
            batch.pxJobs = JobOrder_EDF_W_Test(batch.pxJobs, batch.xJobCount, weight);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "a + E") == 0) {
            batch.pxJobs = JobOrder_FCFS(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(check_if_precedence(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10) == -1) {
                printf("Neizvodiljivo zbog predhodjenja za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "C + E") == 0) {
            batch.pxJobs = JobOrder_SJF(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(check_if_precedence(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10) == -1) {
                printf("Neizvodiljivo zbog predhodjenja za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "d + E") == 0) {
            batch.pxJobs = JobOrder_EDF(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(check_if_precedence(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10) == -1) {
                printf("Neizvodiljivo zbog predhodjenja za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "d + W * C + E") == 0) {
            batch.pxJobs = JobOrder_EDF_W_SJF(batch.pxJobs, batch.xJobCount, weight);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(check_if_precedence(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10) == -1) {
                printf("Neizvodiljivo zbog predhodjenja za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "Test + E") == 0) {
            batch.pxJobs = JobOrder_Test(batch.pxJobs, batch.xJobCount);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(check_if_precedence(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10) == -1) {
                printf("Neizvodiljivo zbog predhodjenja za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
        if(strcmp(heruistik_tmp, "d + W * Test + E") == 0) {
            batch.pxJobs = JobOrder_EDF_W_Test(batch.pxJobs, batch.xJobCount, weight);
            if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
                printf("Neizvodljivo za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(check_if_precedence(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10) == -1) {
                printf("Neizvodiljivo zbog predhodjenja za heruistiku: %s\n", heruistik_tmp);
                flush_all();
                flag_doable = -1;
            }
            if(flag_doable == 1) {
                xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
                vTaskStartScheduler();
            }
        }
    }




}

struct JobType_t* JobOrder_FCFS(struct JobType_t* jobs, int jobCount)
{
    struct JobType_t *result = (struct JobType_t*)malloc(jobCount * sizeof(struct JobType_t));

    int i, j;

    for(i = 0; i < jobCount; i++) {
        result[i] = jobs[i];
    }

    for(i = 0; i < jobCount; i++) {
        for (j = 0; j < jobCount-i-1; j++) {
            TickType_t t_i = result[j].xArrival;
            TickType_t t_j = result[j + 1].xArrival;
            if(t_i > t_j) {
                struct JobType_t tim_job = result[j];
                result[j] = result[j + 1];
                result[j + 1] = tim_job;
            }
        }
    }

    return result;
}

struct JobType_t* JobOrder_SJF(struct JobType_t* jobs, int jobCount) {
    struct JobType_t *result = (struct JobType_t*)malloc(jobCount * sizeof(struct JobType_t));

    int i, j;

    for(i = 0; i < jobCount; i++) {
        result[i] = jobs[i];
    }

    for(i = 0; i < jobCount; i++) {
        for (j = 0; j < jobCount-i-1; j++) {
            TickType_t t_i = result[j].xC;
            TickType_t t_j = result[j + 1].xC;
            if(t_i > t_j) {
                struct JobType_t tim_job = result[j];
                result[j] = result[j + 1];
                result[j + 1] = tim_job;
            }
        }
    }

    return result;
}

struct JobType_t* JobOrder_EDF(struct JobType_t* jobs, int jobCount)
{
    struct JobType_t *result = (struct JobType_t*)malloc(jobCount * sizeof(struct JobType_t));

    int i, j;

    for(i = 0; i < jobCount; i++) {
        result[i] = jobs[i];
    }

    for(i = 0; i < jobCount; i++) {
        for (j = 0; j < jobCount-i-1; j++) {
            TickType_t t_i = result[j].xDeadline;
            TickType_t t_j = result[j + 1].xDeadline;
            if(t_i > t_j) {
                struct JobType_t tim_job = result[j];
                result[j] = result[j + 1];
                result[j + 1] = tim_job;
            }
        }
    }

    return result;
}


struct JobType_t* JobOrder_EDF_W_SJF(struct JobType_t* jobs, int jobCount, double weight) {
    struct JobType_t *result = (struct JobType_t*)malloc(jobCount * sizeof(struct JobType_t));

    int i, j;

    for(i = 0; i < jobCount; i++) {
        result[i] = jobs[i];
    }

    for(i = 0; i < jobCount; i++) {
        for (j = 0; j < jobCount-i-1; j++) {
            TickType_t t_i = result[j].xDeadline + floor((result[j].xC * weight));
            TickType_t t_j = result[j + 1].xDeadline + floor((result[j].xC * weight));
            if(t_i > t_j) {
                struct JobType_t tim_job = result[j];
                result[j] = result[j + 1];
                result[j + 1] = tim_job;
            }
        }
    }

    return result;
}


struct JobType_t* JobOrder_Test(struct JobType_t* jobs, int jobCount) {
    struct JobType_t *result = (struct JobType_t*)malloc(jobCount * sizeof(struct JobType_t));

    int i, j;

    for(i = 0; i < jobCount; i++) {
        result[i] = jobs[i];
    }

    int r;
    for(i = 0; i < jobCount; i++) {
        TickType_t resource_start_i = 0;
        int resource_start_i_id = -1;
        for(r = 0; r < result[i].resource_count; r++) {
            if(result[i].resources[r].timeOfStartAvailability > resource_start_i) {
                resource_start_i = result[i].resources[r].timeOfStartAvailability;
                resource_start_i_id = r;
            }
        }
        for (j = 0; j < jobCount-i-1; j++) {
            TickType_t resource_start_j = 0;
            int resource_start_j_id = -1;
            for(r = 0; r < result[j].resource_count; r++) {
                if(result[j].resources[r].timeOfStartAvailability > resource_start_i) {
                    resource_start_j = result[j].resources[r].timeOfStartAvailability;
                    resource_start_j_id = r;
                }
            }
            TickType_t t_i;
            TickType_t t_j;
            if(resource_start_i > result[j].xArrival) {
                t_i = resource_start_i;
            } else {
                t_i = result[j].xArrival;
            }

            if(resource_start_j > result[j + 1].xArrival) {
                t_j = resource_start_j;
            } else {
                t_j = result[j + 1].xArrival;
            }

            if(t_i > t_j) {
                struct JobType_t tim_job = result[j];
                result[j] = result[j + 1];
                result[j + 1] = tim_job;
            }
        }
    }

    return result;
}


struct JobType_t* JobOrder_EDF_W_Test(struct JobType_t* jobs, int jobCount, double weight) {
    struct JobType_t *result = (struct JobType_t*)malloc(jobCount * sizeof(struct JobType_t));

    int i, j;

    for(i = 0; i < jobCount; i++) {
        result[i] = jobs[i];
    }

    int r;
    for(i = 0; i < jobCount; i++) {
        TickType_t resource_start_i = 0;
        int resource_start_i_id = -1;
        for(r = 0; r < result[i].resource_count; r++) {
            if(result[i].resources[r].timeOfStartAvailability > resource_start_i) {
                resource_start_i = result[i].resources[r].timeOfStartAvailability;
                resource_start_i_id = r;
            }
        }
        resource_start_i = floor(resource_start_i * weight);
        for (j = 0; j < jobCount-i-1; j++) {
            TickType_t resource_start_j = 0;
            int resource_start_j_id = -1;
            for(r = 0; r < result[j].resource_count; r++) {
                if(result[j].resources[r].timeOfStartAvailability > resource_start_i) {
                    resource_start_j = result[j].resources[r].timeOfStartAvailability;
                    resource_start_j_id = r;
                }
            }
            resource_start_j = floor(resource_start_j * weight);

            TickType_t t_i;
            TickType_t t_j;
            if(resource_start_i + result[j].xDeadline > result[j].xArrival) {
                t_i = resource_start_i + result[j].xDeadline;
            } else {
                t_i = result[j].xArrival;
            }

            if(resource_start_j + result[j + 1].xDeadline > result[j + 1].xArrival) {
                t_j = resource_start_j + result[j + 1].xDeadline;
            } else {
                t_j = result[j + 1].xArrival;
            }

            if(t_i > t_j) {
                struct JobType_t tim_job = result[j];
                result[j] = result[j + 1];
                result[j + 1] = tim_job;
            }
        }
    }

    return result;
}


void vJobWrapper(void* pvParameters)
{
    struct JobType_t* myJob = (struct JobType_t*)pvParameters;

    myJob->cState = RUNNING_STATE;

    myJob->func(myJob->pvParameters);
    TickType_t tick = xTaskGetTickCount();
    printf("JOB[%d]  TICK: %d\n", myJob->job_id, tick);
    flush_all();

    myJob->cState = FINISHED_STATE;
    vTaskDelete(0);
}

void vScheduler(void* pvParameters)
{
    struct BatchType_t* jobs = (struct BatchType_t*)pvParameters;
    int i;

    for(i=0; i < jobs->xJobCount; i++)
    {
        //kreiramo FreeRTOS task za job i
        xTaskCreate(vJobWrapper, "", configMINIMAL_STACK_SIZE, jobs->pxJobs + i, 1, &(jobs->pxJobs[i].xHandle));
    }

    for(i=0; i < jobs->xJobCount; i++)
    {
        int currentTask = i;
        vTaskPrioritySet(jobs->pxJobs[currentTask].xHandle, 4);
    }
    printf("terminate\n");
    flush_all();
    printf("terminate\n");
    vTaskDelete(0);
}

void vJobCreate(struct JobType_t *job, int job_id, void (*func)(void*), void* pvParameters,
                TickType_t arrival, TickType_t deadline, TickType_t C)
{
    job -> func = func;
    job -> pvParameters = pvParameters;
    job -> job_id = job_id;
    job -> xArrival = arrival;
    job -> xDeadline = deadline;
    job-> xC = C;
    job -> cState = NEW_STATE;
    job -> xHandle = NULL;
}

void vJobAssingResource(struct JobType_t *job, struct Resource *resources, int resource_count) {
    job -> resources = resources;
    job -> resource_count = resource_count;
}

void vResourceCreate(struct Resource *resource, TickType_t timeAvailability) {
    resource->timeOfStartAvailability = timeAvailability;
}

void jobWork(void* pvParameters)
{
    char* msg = (char*)pvParameters;
    int i;

    for(i=0; i<20; i++)
    {
        printf(msg);
        fflush(stdout);
    }
    printf("\n");
    fflush(stdout);

}

int check_if_precedence(struct JobType_t* jobs, int jobCount,  int* piPrecedenceEdges, int edgesCount) {
    int i, j;

    int completed_jobs[jobCount + 1];
    for(i = 0; i < jobCount + 1; i++) {
        completed_jobs[i] = -1;
    }

    for(i = 0; i < jobCount; i++) {
        for(j = 1; j < edgesCount; j += 2) {
            if(piPrecedenceEdges[j] == jobs[i].job_id + 1) {
                if(completed_jobs[piPrecedenceEdges[j - 1]] != 1) {
                    printf("Breaks on: {job_id: %d}, {precedence_index: %d}\n", jobs[i].job_id, j - 1);
                    flush_all();
                    return -1;
                }
            }
        }
        completed_jobs[jobs[i].job_id + 1] = 1;
    }

    return 1;
}

int check_if_doable(struct JobType_t* jobs, int jobCount) {
    int i;
    TickType_t doing_tick = 0;
    for(i = 0; i < jobCount; i++) {
        doing_tick = doing_tick + jobs[i].xC;
        if (doing_tick > jobs[i].xDeadline) {
            printf("Doing tick: %d - jobs d: %d\nJOB[%d]\n", doing_tick, jobs[i].xC, jobs[i].job_id);
            return -1;
        }
    }

    return 1;
}

void flush_all() {
    fflush(stdin);
    fflush(stdout);
}


//    struct Resource r1, r2, r3;
//    vResourceCreate(&r1, 21);
//    vResourceCreate(&r2, 3);
//    vResourceCreate(&r3, 7);
//
//    struct Resource *resources = (struct Resource*) malloc(3 * sizeof(struct Resource));
//    resources[0] = r1;
//    resources[1] = r2;
//    resources[2] = r3;
//
//    struct JobType_t job1, job2, job3, job4, job5, job6;
//    vJobCreate(&job1, 0, jobWork, ".", 10, 20, 5);
//    vJobCreate(&job2, 1, jobWork, "-", 10, 50, 5);
//    vJobCreate(&job3, 2, jobWork, "|", 11, 40, 6);
//    vJobCreate(&job4, 3, jobWork, "*", 20, 30, 100);
//    vJobCreate(&job5, 4, jobWork, "?", 5, 50, 90);
//    vJobCreate(&job6, 5, jobWork, "%%", 80, 60, 80);
//
//    vJobAssingResource(&job1, resources, 3);
//    vJobAssingResource(&job2, resources, 3);
//    vJobAssingResource(&job3, resources, 3);
//    vJobAssingResource(&job4, resources, 3);
//    vJobAssingResource(&job5, resources, 3);
//    vJobAssingResource(&job6, resources, 3);
//
//    struct BatchType_t batch;
//    batch.pxJobs = (struct JobType_t*)malloc(6 * sizeof(struct JobType_t));
//    batch.xJobCount = 6;
//    batch.pxJobs[0] = job1;
//    batch.pxJobs[1] = job2;
//    batch.pxJobs[2] = job3;
//    batch.pxJobs[3] = job4;
//    batch.pxJobs[4] = job5;
//    batch.pxJobs[5] = job6;
//
//    int piPrecedenceEdges[] = {1, 2, 1, 3, 2, 4, 2, 5, 3, 6};
//
//    batch.pxJobs = JobOrder_EDF(batch.pxJobs, batch.xJobCount);
//
//    if(check_if_precedence(batch.pxJobs, batch.xJobCount, piPrecedenceEdges, 10) == -1) {
//        printf("NE MOZE BATO\n");
//        flush_all();
//        return 0;
//    }
//
//    if(check_if_doable(batch.pxJobs, batch.xJobCount) == -1) {
//        printf("NE MOZE BATO\n");
//        flush_all();
//        return 0;
//    }
//
//    xTaskCreate(vScheduler, "", configMINIMAL_STACK_SIZE, &batch, 3, NULL);
//
//    vTaskStartScheduler();
