//
//  reg.h
//  iVM
//
//  Created by TobyMars on 2021/3/4.
//  Copyright © 2021 TobyMars. All rights reserved.
//

#ifndef reg_h
#define reg_h

#include "common.h"

#include "config.h"

typedef uint32_t reg_t;

/**
 * A dynamic array of regs
 */
typedef struct {
    reg_t *regs;
    int capacity; /* the number of elements in the array we have allocated */
    int count; /* how many of those allocated entries are actually in use */
} reg_array_t;


void reg_init(reg_array_t *array);
void reg_free(reg_array_t *array);
void reg_write(reg_array_t *array, reg_t reg);
void reg_print(reg_t reg);
void reg_print_all(reg_array_t *array);
reg_t reg_pop(reg_array_t *array);
reg_t reg_pop_as_count(reg_array_t *array, int count);

void reg_test(void);


#endif /* reg_h */
