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
static int items_put_in_map = 0;

struct map_item {
    long key;
    long *factor_values;
    struct map_item *next;
};

struct map_table_item {
    int index;
    struct map_item *map_item_head;
    struct map_table_item *next;
};

struct map_table_item *head = NULL;

int iHashFunction(int number);

void vFactorizeNumber(int number, int *factorsArrayStore);

int iMapPut(long key, long *factor_values);

int check_for_collision(int index);

void insert_into_map_table(struct map_table_item *table_item);

void insert_into_map_item(struct map_item *map_item_head, struct map_item *item);

int check_if_element_exists(struct map_item *map_item_head, long key);

struct map_table_item *get_table_item(int index);

void flush_all();

void print_map();


int domaci_I(void) {

    long test_array[5] = {1, 2, 3, 4, 5};
    printf("Enter bound: ");
    flush_all();
    long a, b;
    scanf("%ld %ld", &a, &b);
    while(a <= b)
        iMapPut(a++, test_array);
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

int iHashFunction(int number) {
    return (number * 21315421) % MAX_MAP_SIZE;
}

void vFactorizeNumber(int number, int *factorsArrayStore) {
	int divisor = 2;

}

int iMapPut(long key, long *factor_values) {
    if(items_put_in_map >= MAX_MAP_SIZE) {
        printf("Map full, unable to add element. {%ld}\n", key);
        flush_all();
        return -1;
    }

    struct map_item *item = NEW_ALLOCATE(struct map_item);
    item->key = key;
    item->factor_values = factor_values;
    item->next = NULL;

    int index = iHashFunction((int)key);

    if(!head) {
        struct map_table_item *table_item = NEW_ALLOCATE(struct map_table_item);
        table_item->index = index;
        table_item->map_item_head = item;
        table_item->next = NULL;

        head = table_item;
    } else {
        if(check_for_collision(index) == -1) {
            struct map_table_item *table_item = NEW_ALLOCATE(struct map_table_item);
            table_item->index = index;
            table_item->map_item_head = item;
            table_item->next = NULL;

            insert_into_map_table(table_item);
        } else {
            struct map_table_item *table_item = get_table_item(index);
            if(check_if_element_exists(table_item->map_item_head, item->key) == -1) {
                insert_into_map_item(table_item->map_item_head, item);
            }
            else
                return -2;
        }
    }

    items_put_in_map++;
    return 1;
}

int check_for_collision(int index) {
    struct map_table_item *tmp = head;

    while(tmp != NULL) {
        if(tmp->index == index)
            return 1;
        tmp = tmp->next;
    }

    return -1;
}

void insert_into_map_table(struct map_table_item *table_item) {
    struct map_table_item *tmp = head;

    while(tmp->index > table_item->index || tmp->next != NULL)
        tmp = tmp->next;

    table_item->next = tmp->next;
    tmp->next = table_item;
}

void insert_into_map_item(struct map_item *map_item_head, struct map_item *item) {
    struct map_item *tmp = map_item_head;

    while(tmp->next != NULL)
        tmp = tmp->next;

    tmp->next = item;
}

int check_if_element_exists(struct map_item *map_item_head, long key) {
    struct map_item *tmp = map_item_head;

    while(tmp != NULL) {
        if(tmp->key == key) {
            return 1;
        }
        tmp = tmp->next;
    }

    return -1;
}

struct map_table_item *get_table_item(int index) {
    struct map_table_item *tmp = head;

    while(tmp->next != NULL) {
        if(tmp->index == index)
            return tmp;
        tmp = tmp->next;
    }

    return NULL;
}

void flush_all() {
    fflush(stdout);
    fflush(stdin);
}

void print_map() {
    struct map_table_item *tmp = head;

    struct map_item *item_tmp;
    while(tmp != NULL) {
        item_tmp = tmp->map_item_head;
        printf("[MAP_TABLE_ITEM] {%d}:\n", tmp->index);
        flush_all();
        while(item_tmp != NULL) {
            printf("[MAP_ITEM] {KEY: %ld}\t", item_tmp->key);
            flush_all();
            item_tmp = item_tmp->next;
        }
        tmp = tmp->next;
        printf("\n");
        flush_all();
    }

}
