
#include "value.h"

#include "memory.h"

#include "debug.h"


void
value_init(value_array_t *array)
{
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void
value_free(value_array_t *array)
{
    MEM_FREE_ARRAY(value_t, array->values, array->capacity);
    value_init(array);
}

void
value_write(value_array_t *array, value_t value)
{
    value_array_t *a = array;
    
    if (a->capacity < a->count + 1) {
        int old_capacity = a->capacity;
        a->capacity = MEM_GROW_CAPACITY(old_capacity);
        a->values = MEM_GROW_ARRAY(value_t, a->values, old_capacity, a->capacity);
    }
    
    a->values[a->count] = value;
    a->count++;
}

void
value_print(value_t value)
{
    printf("%g", value);
}

void
value_test(void)
{
    fs();
    
    value_array_t array;
    
    value_init(&array);
    value_write(&array, 1.0);
    value_free(&array);
}
