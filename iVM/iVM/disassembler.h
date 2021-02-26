

#ifndef disassembler_h
#define disassembler_h

#include "common.h"

#include "chunk.h"

bool disassemble_load(const char *fname, chunk_t *chunk);

void disassemble_chunk(chunk_t *chunk, const char *name);

int disassemble_instruction(chunk_t *chunk, int offset);

void disassemble_test(void);


#endif /* disassembler_h */
