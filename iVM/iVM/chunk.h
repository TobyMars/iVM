
#ifndef chunk_h
#define chunk_h


#include "common.h"
#include "value.h"
#include "register.h"
#include "instruction.h"

/**
 * A dynamic array of instructions
 */
typedef struct {
    instruction_t *code; /* instruction array */
    int *lines; /* line information for debugging, it exactly parallels the bytecode array */
    int capacity; /* the number of elements in the array we have allocated */
    int count; /* how many of those allocated entries are actually in use */
    
    value_array_t constants;
    reg_array_t reg_array;
} chunk_t;


void chunk_init(chunk_t *chunk);
void chunk_free(chunk_t *chunk);
void chunk_write(chunk_t *chunk, instruction_t instructin, int line);
int chunk_add_constant(chunk_t *chunk, value_t value);
int chunk_add_reg(chunk_t *chunk, reg_t reg);

void chunk_test(void);


#endif /* chunk_h */


