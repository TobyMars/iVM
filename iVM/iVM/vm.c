

#include "vm.h"

#include "debug.h"
#include "mem.h"
#include "util.h"
#include "error.h"
#include "value.h"
#include "disassembler.h"


/*
 ## Demo2
 
 Write a demo to support below features:
 
 - Athematic Operation
 - Register-based vm
 - lisp-based syntax
 */

static vm_t vm = { 0 };

vm_ret_t
vm_init(void)
{
    memset(&vm, 0, sizeof(vm_t));
    
    out(ok, VM_OK);
}


void
vm_free(void)
{
}

#define arithmetic_op(instruction, op) \
do { \
int start = (instruction >> 8) & 0xFF; \
int end = instruction & 0xFF; \
debug("instruction:0x%08x \n", instruction); \
debug("index start:%d, index end:%d \n", start, end); \
reg_t r = 0; \
reg_array_t *array = &vm.chunk->reg_array; \
reg_t reg_a = array->regs[start]; \
reg_t reg_b = array->regs[end]; \
debug("register value: {\n"); \
reg_print(reg_a); \
reg_print(reg_b); \
r = reg_a op reg_b; \
debug("}\n"); \
reg_print(r); \
array->regs[start] = r; \
} while(0)

#define READ_INSTUCTION() (*vm.ip++)

static vm_ret_t
run(void)
{

    instruction_t instruction;
    value_t constant;
    reg_array_t *array = &vm.chunk->reg_array;
    while (1) {
        
#ifdef DEBUG_TRACE_EXECUTION
        int offset = (int)(vm.ip - vm.chunk->code);
        disassemble_instruction(vm.chunk, offset);
        reg_print_all(array);
        debug("\n");
#endif
        
        instruction = READ_INSTUCTION();
        opcode_t opcode = i_decode_opcode(instruction);
        switch (opcode) {
            case OP_RETURN:
                reg_print(array->regs[0]);
                return VM_OK;
            case OP_ADD: arithmetic_op(instruction, +); break;
            case OP_SUBTRACT: arithmetic_op(instruction, -); break;
            case OP_MULTIPLY: arithmetic_op(instruction, *); break;
            case OP_DIVIDE:   arithmetic_op(instruction, /); break;
                
            default:
                return VM_INTERPRET_RUNTIME_ERROR;
        }
    }
    
    return VM_OK;
}

vm_ret_t
vm_interpret(chunk_t *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}




