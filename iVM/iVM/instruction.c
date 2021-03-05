//
//  instruction.c
//  iVM
//
//  Created by TobyMars on 2021/3/3.
//  Copyright © 2021 TobyMars. All rights reserved.
//

#include "instruction.h"

#include "mem.h"
#include "debug.h"


instruction_t
i_encode_return(void)
{
    instruction_t i;
    
    i = ENCODE_OPCODE(OP_RETURN);
    
    debug("RETURN instruction: 0x%08x \n", i);
    
    return i;
}

instruction_t
i_encode_constant(uint8_t constant_index)
{
    instruction_t i;
    
    i = ENCODE_OPCODE(OP_CONSTANT);
    i |= (0xFF & constant_index);
    
    debug("CONSTANT instruction: 0x%08x \n", i);
    
    return i;
}

instruction_t
i_encode_add(uint8_t reg_index_start, uint8_t reg_index_end)
{
    instruction_t i;
    
    i = ENCODE_OPCODE(OP_ADD);
    i |= ((reg_index_start << 8) | reg_index_end);
    
    debug("ADD instruction: 0x%08x \n", i);
    
    return i;
}

instruction_t
i_encode_sub(uint8_t reg_index_start, uint8_t reg_index_end)
{
    instruction_t i;
    
    i = ENCODE_OPCODE(OP_SUBTRACT);
    i |= ((reg_index_start << 8) | reg_index_end);
    
    debug("SUB instruction: 0x%08x \n", i);
    
    return i;
}

instruction_t
i_encode_multiply(uint8_t reg_index_start, uint8_t reg_index_end)
{
    instruction_t i;
    
    i = ENCODE_OPCODE(OP_MULTIPLY);
    i |= ((reg_index_start << 8) | reg_index_end);
    
    debug("MULTIPLY instruction: 0x%08x \n", i);
    
    return i;
}

instruction_t
i_encode_divide(uint8_t reg_index_start, uint8_t reg_index_end)
{
    instruction_t i;
    
    i = ENCODE_OPCODE(OP_DIVIDE);
    i |= ((reg_index_start << 8) | reg_index_end);
    
    debug("DIVIDE instruction: 0x%08x \n", i);
    
    return i;
}

opcode_t
i_decode_opcode(instruction_t instruction)
{
    return (instruction >> 24) & 0xFF;
}
