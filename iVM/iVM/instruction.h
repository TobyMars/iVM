//
//  instruction.h
//  iVM
//
//  Created by TobyMars on 2021/3/3.
//  Copyright © 2021 TobyMars. All rights reserved.
//

#ifndef instruction_h
#define instruction_h

#include "common.h"

#include "config.h"



/*
 ** Instruction Format:
 ** Opcode  Operand
 **
 ** OP_RETURN(1B)   nil(3B)
 ** OP_CONSTANT(1B) nil(2B)     constant-index(1B)
 ** OP_NEGATE(1B)   nil(3B)
 ** OP_ADD(1B)      nil(1B)     start-reg-index(1B) end-reg-index(1B)
 **
 */

/*
 ** Opcodes
 */
typedef enum {
    OP_UNKNOWN,
    OP_CONSTANT, /* load constant */
    OP_NEGATE, /* get the negative value */
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN, /* return from the current function */
} opcode_type_t;

#define OS_32BIT

#ifdef OS_32BIT
typedef uint32_t instruction_t;
#else
typedef uint64_t instruction_t;
#endif

typedef uint8_t opcode_t;

#define INSTRUCTION_SIZE (sizeof(instruction_t) << 3)
#define OPCODE_SIZE (sizeof(opcode_t) << 3)
#define ENCODE_OPCODE(c) ((c) << (INSTRUCTION_SIZE - OPCODE_SIZE))

instruction_t i_encode_return(void);
instruction_t i_encode_constant(uint8_t constant_index);
instruction_t i_encode_add(uint8_t reg_index_start, uint8_t reg_index_end);
instruction_t i_encode_sub(uint8_t reg_index_start, uint8_t reg_index_end);
instruction_t i_encode_multiply(uint8_t reg_index_start, uint8_t reg_index_end);
instruction_t i_encode_divide(uint8_t reg_index_start, uint8_t reg_index_end);
opcode_t i_decode_opcode(instruction_t instruction);

#endif /* instruction_h */
