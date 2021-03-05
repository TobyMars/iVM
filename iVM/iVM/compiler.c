//
//  compiler.c
//  iVM
//
//  Created by TobyMars on 2021/2/16.
//  Copyright © 2021 TobyMars. All rights reserved.
//

#include "compiler.h"

#include "debug.h"
#include "error.h"
#include "stack.h"
#include "mem.h"

#include "disassembler.h"

static compiler_t *c = NULL;

static compiler_ret_t
compile_func_form(form_s *form)
{
    func_s();
    
    lisp_list_s *l;
    
    if (!form->list->next) {
        
        debug("null form \n");
        return COMPILER_ERR;
    }
    
    l = form->list->next;
    
    debug("list form \n");
    list_show(form->list);
    
    /* ignore '(' */
    l = l->next;
    
    char *name = l->obj.token.value.symbol;
    debug("name: %s \n", name);
    
    /* Instruction Format(Length 32bit):
     * Operator(1B)  Operands(3B)
     *
     * Register x(index 32bit):
     * Rx(double)
     *
     * Constant y(index 32bit):
     * Ky(double)
     *
     * Variable z(index 32bit):
     * Vz(double)
     *
     * (+ 1 2)
     * ADD      (1B)
     * 1        (1B)
     * 2        (2B)
     *
     * push     ADD-1-2
     * pop      ADD-1-2
     * CALL     R0:ADD-1-2      R0:3
     * RET      R0:3
     * 3
     
     * (+ 256 (- 2 3))
     * ADD      (1B)
     * 256      (2B)
     * R0       (1B):RET (- 2 3)
     * SUB      (1B)
     * 2        (1B)
     * 3        (2B)
     *
     * push     ADD-256-R0
     * push     SUB-2-3
     * pop      SUB-2-3
     * CALL     R0:SUB-2-3      R0:-1
     * pop      ADD-256-R0
     * CALL     R0:ADD-256-R0   R0:255
     * RET      R0:255
     * 255
     *
     * (+ (* 2 128) (- 2 3))
     * ADD      (1B)
     * R0       (2B):RET (* 2 128)
     * R1       (1B):RET (- 2 3)
     * ...
     * push     ADD-R0-R1
     * push     SUB-2-3
     * push     MUL-2-128
     * pop      MUL-2-128
     * CALL     R0:MUL-2-128    R0:256
     * pop      SUB-2-3
     * CALL     R1:SUB-2-3      R1:-1
     * pop      ADD-R0-R1
     * CALL     R0:ADD-R0-R1    R0:255
     * RET      R0
     * 255
     *
     */
    
    
    /* push function name */
    uint8_t operator;
    instruction_t i = 0;
    int r_index = -1;
    int r_index_end = -1;
    
    chunk_t *chunk = &c->chunk;
    int line = 1; /* TODO */
    
    /* push arguments */
    value_t v;
    l = l->next;
    while (l) {
        object_s *obj = &l->obj;
        if (obj_is_number(obj)) {
            obj_show(obj);
            v = (value_t)token_get_fixnum(&obj->token);
            if (r_index == -1) {
                r_index = chunk_add_reg(chunk, v);
            }
            else {
                r_index_end = chunk_add_reg(chunk, v);
            }
        }
        else if (obj_is_list(obj)) {
            
            if (r_index == -1) {
                r_index = chunk->reg_array.count;
            }
            else {
                r_index_end = chunk->reg_array.count;
            }
            
            //            form_s *f = l->obj.sub;
            //            form_show(f);
            //            lisp_list_s *list = form_get_list(l->obj.sub);
            //            list_show(list);
            form_s *f = l->obj.sub;
            compile_func_form(f->next);
        }
        else {
            debug_err("Unknown obj \n");
            obj_show(obj);
            goto FAIL;
        }
        
        l = l->next;
        if (l->next && l->next->is_head) break;
    }
    
    debug("r_index:%d, r_index_end:%d \n", r_index, r_index_end);
    if (!strcasecmp(name, "+")) {
        operator = OP_ADD;
        debug("OP_ADD \n");
        i = i_encode_add(r_index, r_index_end);
    }
    else if (!strcasecmp(name, "-")) {
        operator = OP_SUBTRACT;
        debug("OP_SUBTRACT \n");
        i = i_encode_sub(r_index, r_index_end);
    }
    else if (!strcasecmp(name, "*")) {
        operator = OP_MULTIPLY;
        debug("OP_MULTIPLY \n");
        i = i_encode_multiply(r_index, r_index_end);
    }
    else if (!strcasecmp(name, "/")) {
        operator = OP_DIVIDE;
        debug("OP_DIVIDE \n");
        i = i_encode_divide(r_index, r_index_end);
    }
    else {
        debug_err("Unknown name:%s \n", name);
        goto FAIL;
    }
    
    chunk_write(chunk, i, line);
    
DONE:
    out(ok, COMPILER_OK);
    
FAIL:
    out(fail, COMPILER_ERR);
}

bool
compile_init(compiler_t *compiler)
{
    if (!compiler) return false;
    
    memset(compiler, 0, sizeof(compiler_t));
    
    chunk_init(&compiler->chunk);
    
    c = compiler;
    
    return true;
}


compiler_ret_t
compile(form_s *form)
{
    compiler_ret_t rt;
    
    if (!form) return COMPILER_ERR_NULL;
    
    func_s();
    
    form_s *f = form->next;
    
    while (f && f != form) {
        
        switch (f->type) {
                
            case COMPOUND_SPECIAL_FORM:
                break;
                
            case COMPOUND_FUNCTION_FORM:
                rt = compile_func_form(f);
                if (rt != COMPILER_OK) goto FAIL;
                break;
                
            case SYMBOL_FORM:
                break;
                
            case SELF_EVALUATING_FORM:
                break;
                
            case COMPOUND_MACRO_FORM:
                break;
                
            default:
                
                debug_err("unknown form type %d \n", f->type);
                goto FAIL;
        }
        
        chunk_write(&c->chunk, i_encode_return(), 666);
        
        f = f->next;
    }
    
    out(ok, COMPILER_OK);
    
FAIL:
    func_fail();
    ml_err_signal(ML_ERR_COMPILER);
    return COMPILER_ERR;
}

