
#ifndef ML_VM_H
#define ML_VM_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "chunk.h"

typedef enum
{
    VM_OK,
    VM_ERR,
    VM_NULL,
    VM_NO_MEM,
    VM_INTERPRET_COMPILE_ERROR,
    VM_INTERPRET_RUNTIME_ERROR
} vm_ret_t;

typedef struct
{
    chunk_t *chunk;
    instruction_t *ip;
} vm_t;


vm_ret_t vm_init(void);
void vm_free(void);

vm_ret_t vm_interpret(chunk_t *chunk);


#endif /* ML_VM_H */

