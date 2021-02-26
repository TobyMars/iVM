

#include "vm.h"

#include "debug.h"
#include "mem.h"
#include "util.h"
#include "error.h"
#include "value.h"
#include "disassembler.h"


/*
 ## Demo1
 
 Write a demo to support below features:
 
 - Athematic Operation
 - Stack-based vm
 - lisp-based syntax
 */

static vm_t vm = { 0 };


static void
stack_reset(void)
{
    vm.stack_top = vm.stack;
}

static void
stack_push(value_t value)
{
    *vm.stack_top = value;
    vm.stack_top++;
}

static value_t
stack_pop(void)
{
    vm.stack_top--;
    return *vm.stack_top;
}

vm_ret_t
vm_init(void)
{
    memset(&vm, 0, sizeof(vm_t));
    
    stack_reset();
    
    out(ok, VM_OK);
}


void
vm_free(void)
{
}


static vm_ret_t
run(void)
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
do { \
double b = stack_pop(); \
double a = stack_pop(); \
stack_push(a op b); \
} while (false)
    
    uint8_t instruction;
    value_t constant;
    while (1) {
        
#ifdef DEBUG_TRACE_EXECUTION
        int offset = (int)(vm.ip - vm.chunk->code);
        disassemble_instruction(vm.chunk, offset);
        debug("offset:%d \n", offset);
        debug("stack ");
        for (value_t *slot = vm.stack; slot < vm.stack_top; slot++) {
            debug("[ ");
            value_print(*slot);
            debug(" ]");
        }
        debug("\n");
#endif
        
        instruction = READ_BYTE();
        switch (instruction) {
            case OP_RETURN:
                value_print(stack_pop());
                debug("\n");
                return VM_OK;
                
            case OP_CONSTANT:
                constant = READ_CONSTANT();
                stack_push(constant);
                debug("\n");
                break;
                
            case OP_NEGATE:   stack_push(-stack_pop()); break;
            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
                
            default:
                return VM_INTERPRET_RUNTIME_ERROR;
        }
    }
    
    return VM_OK;
    
#undef READ_BYTE
#undef READ_CONSTANE
#undef BINARY_OP
}

vm_ret_t
vm_interpret(chunk_t *chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}




