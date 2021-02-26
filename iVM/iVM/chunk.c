

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
}

void
chunk_free(chunk_t *chunk)
{
    MEM_FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    MEM_FREE_ARRAY(int, chunk->lines, chunk->capacity);
    value_free(&chunk->constants);
    chunk_init(chunk);
}

void
chunk_write(chunk_t *chunk, uint8_t byte, int line)
{
    if (chunk->capacity < chunk->count + 1) {
        int old_capacity = chunk->capacity;
        chunk->capacity = MEM_GROW_CAPACITY(old_capacity);
        chunk->code = MEM_GROW_ARRAY(uint8_t, chunk->code,
                                     old_capacity, chunk->capacity);
        chunk->lines = MEM_GROW_ARRAY(int, chunk->lines,
                                      old_capacity, chunk->capacity);
    }
    
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}


int
chunk_add_constant(chunk_t *chunk, value_t value)
{
    value_write(&chunk->constants, value);
    return (chunk->constants.count - 1); /* index in value array */
}

void
chunk_test(void)
{
    fs();
    
    chunk_t chunk;
    
    chunk_init(&chunk);
    chunk_write(&chunk, OP_RETURN, 123);
    chunk_free(&chunk);
}


