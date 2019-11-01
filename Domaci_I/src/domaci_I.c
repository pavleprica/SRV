/*
 * domaci_I.c
 *
 *  Created on: 29.10.2019.
 *      Author: pavle
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define NEW_ALLOCATE(node) (node*)malloc(sizeof(node))


#define MAX_MAP_SIZE 30
#define MAP_ARRAY_SIZE 10
static int items_put_in_map = 0;

struct map_item {
    long key;
    long *factor_values;
    struct map_item *next;
};

struct map_table_item {
    struct map_item *map_item_head;
};

struct map_table_item *map;

int iHashFunction(int number);

void vFactorizeNumber(long number);

int iMapPut(long key, long *factor_values);

void insert_into_map_item(int index, struct map_item *item);

int check_if_element_exists(int index, long key);

struct map_table_item *get_table_item(int index);

void flush_all();

void print_map();

void init_map_array();

int iMapSize();

void add_factor_to_array(long *factorsArrayStore, int size, int divisor);


int domaci_I(void) {
    init_map_array();

    printf("Enter bound: ");
    flush_all();
    long a, b;
    scanf("%ld %ld", &a, &b);
    while(a <= b)
        vFactorizeNumber(a++);

//    iMapPut(5, test_array);
//    iMapPut(6, test_array);
//    iMapPut(5, test_array);
//    iMapPut(15, test_array);
//    iMapPut(25, test_array);
//    iMapPut(306, test_array);
//    iMapPut(5, test_array);
//    iMapPut(5, test_array);
//    iMapPut(5, test_array);
//    iMapPut(5, test_array);
//    iMapPut(5, test_array);
//    iMapPut(100, test_array);
//    iMapPut(101, test_array);
//    iMapPut(102, test_array);
//    iMapPut(103, test_array);
//    iMapPut(104, test_array);

    print_map();
    return 0;
}

void init_map_array() {
    map = (struct map_table_item*)malloc(MAP_ARRAY_SIZE * sizeof(struct map_table_item));
    int i;
    for(i = 0; i < MAP_ARRAY_SIZE; i++) {
        map[i].map_item_head = NULL;
    }
}

int iHashFunction(int number) {
    return (number * 21315421) % MAP_ARRAY_SIZE;
}

void add_factor_to_array(long *factorsArrayStore, int size, int divisor) {
    long *factorTmp = factorsArrayStore;

    factorsArrayStore = (long *) realloc(factorsArrayStore, size * sizeof(long));
    *(factorsArrayStore - (size - 2)) = (long) divisor;
    *(factorsArrayStore - (size - 1)) = -1;
}

void vFactorizeNumber(long number) {
    long num_holder = number;
	int divisor = 2, size = 1;
    long *factorsArrayStore;

	factorsArrayStore = (long *) malloc(sizeof(long));
	*factorsArrayStore = -1;

	while(number > 1) {
	    if(number % divisor == 0) {
	        printf("Entered condition {number_holder: %ld, number: %ld, divisor: %d, size: %d}\n", num_holder, number, divisor, size);
	        flush_all();
	        size++;
	        add_factor_to_array(factorsArrayStore, size, divisor);
            printf("FACTORISED\n");
            flush_all();
	        number = number / divisor;
//	        if(num_in_factorised()) {
//
//	        }
	    } else
	        divisor++;
	}
	iMapPut(num_holder, factorsArrayStore);
}

int iMapPut(long key, long *factor_values) {
    if(iMapSize() >= MAX_MAP_SIZE) {
        printf("Map full, unable to add element. {%ld}\n", key);
        flush_all();
        return -1;
    }

    struct map_item *item = NEW_ALLOCATE(struct map_item);
    item->key = key;
    item->factor_values = factor_values;
    item->next = NULL;

    int index = iHashFunction((int)key);

    if(map[index].map_item_head == NULL) {
        struct map_table_item table_item = {.map_item_head = item};
        map[index] = table_item;
    } else {
        if(check_if_element_exists(index, item->key) == -1)
            insert_into_map_item(index, item);
        else return -2;
    }

    items_put_in_map++;
    return 1;
}


void insert_into_map_item(int index, struct map_item *item) {
    struct map_item *tmp = map[index].map_item_head;

    while(tmp->next != NULL)
        tmp = tmp->next;

    tmp->next = item;
}

int check_if_element_exists(int index, long key) {
    struct map_item *tmp = map[index].map_item_head;

    while(tmp != NULL) {
        if(tmp->key == key) {
            return 1;
        }
        tmp = tmp->next;
    }

    return -1;
}

void flush_all() {
    fflush(stdout);
    fflush(stdin);
}

void print_map() {
	int i, j;

	for(i = 0; i < MAP_ARRAY_SIZE; i++) {
        struct map_item *item_tmp = map[i].map_item_head;
        printf("[MAP_TABLE_ITEM] {%d}:\n", i);
        flush_all();
        while(item_tmp != NULL) {
            printf("[MAP_ITEM] {KEY: %ld} {", item_tmp->key);
            j = 0;
            while(*(item_tmp->factor_values + j) != -1) {
                printf("%ld ", *(item_tmp->factor_values + j++));
            }
            printf("}\t");
            flush_all();
            item_tmp = item_tmp->next;
        }
        printf("\n");
        flush_all();
	}

}

int iMapSize() {
    return items_put_in_map;
}
