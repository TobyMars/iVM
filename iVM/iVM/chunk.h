
#ifndef chunk_h
#define chunk_h


#include "common.h"
#include "value.h"


/*
 ** Instruction Format:
 ** Opcode  Operand
 **
 ** 1B: OP_RETURN(1B)
 ** 2B: OP_CONSTANT(1B) constant-index(1B)
 ** 1B: OP_NEGATE(1B)
 **
 */

/*
 ** Opcodes
 */
typedef enum {
    OP_CONSTANT, /* load constant */
    OP_NEGATE, /* get the negative value */
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN, /* return from the current function */
    
    OP_UNKNOWN,
} opcode_t;


/**
 * A dynamic array of instructions
 */
typedef struct {
    uint8_t *code; /* bytecode array */
    int *lines; /* line information for debugging, it exactly parallels the bytecode array */
    int capacity; /* the number of elements in the array we have allocated */
    int count; /* how many of those allocated entries are actually in use */
    
    value_array_t constants;
} chunk_t;


void chunk_init(chunk_t *chunk);
void chunk_free(chunk_t *chunk);
void chunk_write(chunk_t *chunk, uint8_t byte, int line);
int chunk_add_constant(chunk_t *chunk, value_t value);

void chunk_test(void);


#endif /* chunk_h */


