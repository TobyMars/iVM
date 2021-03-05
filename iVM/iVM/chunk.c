

#include "chunk.h"
#include "memory.h"

#include "debug.h"

#include <stdlib.h>


void
chunk_init(chunk_t *chunk)
{
    chunk->code = NULL;
    chunk->lines = NULL;
    chunk->capacity = 0;
    chunk->count = 0;
    
    value_init(&chunk->constants);
    reg_init(&chunk->reg_array);
}

void
chunk_free(chunk_t *chunk)
{
    MEM_FREE_ARRAY(instruction_t, chunk->code, chunk->capacity);
    MEM_FREE_ARRAY(int, chunk->lines, chunk->capacity);
    value_free(&chunk->constants);
    reg_free(&chunk->reg_array);
    chunk_init(chunk);
}

void
chunk_write(chunk_t *chunk, instruction_t instructin, int line)
{
    if (chunk->capacity < chunk->count + 1) {
        int old_capacity = chunk->capacity;
        chunk->capacity = MEM_GROW_CAPACITY(old_capacity);
        chunk->code = MEM_GROW_ARRAY(instruction_t, chunk->code,
                                     old_capacity, chunk->capacity);
        chunk->lines = MEM_GROW_ARRAY(int, chunk->lines,
                                      old_capacity, chunk->capacity);
    }
    
    chunk->code[chunk->count] = instructin;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int
chunk_add_constant(chunk_t *chunk, value_t value)
{
    value_write(&chunk->constants, value);
    return (chunk->constants.count - 1); /* index in value array */
}

int
chunk_add_reg(chunk_t *chunk, reg_t reg)
{
    reg_write(&chunk->reg_array, reg);
    return (chunk->reg_array.count - 1); /* index in register array */
}

void
chunk_test(void)
{
    fs();
    
    chunk_t chunk;
    chunk_t *c = NULL;
    
    chunk_init(&chunk);
    
    c = &chunk;
    
    int index = chunk_add_constant(c, 55);
    chunk_write(c, i_encode_constant(index), 123);
    
    chunk_write(c, i_encode_return(), 123);
    
    chunk_write(c, i_encode_add(1, 2), 123);
    
    chunk_free(c);
}


