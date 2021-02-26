

#ifndef value_h
#define value_h


#include "common.h"


typedef double value_t;

/**
 * A dynamic array of values
 */
typedef struct {
    value_t *values;
    int capacity; /* the number of elements in the array we have allocated */
    int count; /* how many of those allocated entries are actually in use */  
} value_array_t;


void value_init(value_array_t *array);
void value_free(value_array_t *array);
void value_write(value_array_t *array, value_t value);
void value_print(value_t value);
void value_test(void);


#endif /* value_h */
