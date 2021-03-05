//
//  reg.c
//  iVM
//
//  Created by TobyMars on 2021/3/4.
//  Copyright © 2021 TobyMars. All rights reserved.
//

#include "register.h"

#include "debug.h"
#include "memory.h"


void
reg_init(reg_array_t *array)
{
    array->regs = NULL;
    array->capacity = 0;
    array->count = 0;
}

void
reg_free(reg_array_t *array)
{
    MEM_FREE_ARRAY(reg_t, array->regs, array->capacity);
    reg_init(array);
}

void
reg_write(reg_array_t *array, reg_t reg)
{
    reg_array_t *a = array;
    
    if (a->capacity < a->count + 1) {
        int old_capacity = a->capacity;
        a->capacity = MEM_GROW_CAPACITY(old_capacity);
        a->regs = MEM_GROW_ARRAY(reg_t, a->regs, old_capacity, a->capacity);
    }
    
    a->regs[a->count] = reg;
    a->count++;
    //    debug("count:%d \n", a->count);
}

void
reg_print(reg_t reg)
{
    printf("0x%08x \n", reg);
}

void
reg_print_all(reg_array_t *array)
{
    debug("registers: [ ");
    for (int i = 0; i < array->count; i++) {
        printf("0x%08x ", array->regs[i]);
    }
    debug("]\n");
}

reg_t
reg_pop(reg_array_t *array)
{
    reg_t reg = array->regs[array->count-1];
    array->count--;
    return reg;
}

reg_t
reg_pop_as_count(reg_array_t *array, int count)
{
    array->count -= count;
    return array->regs[array->count];
}

void
reg_test(void)
{
    fs();
    
    reg_array_t array;
    
    reg_init(&array);
    reg_write(&array, 0x01020304);
    reg_print(array.regs[0]);
    reg_free(&array);
}


