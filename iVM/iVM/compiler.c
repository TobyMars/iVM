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
    
    /* sequence the arithmetic nodes into vm stack(unlimited count of registers...)
     *
     * (+ 1 2)
     * push + (operator)
     * push 1 (oprand)
     * push 2 (oprand)
     * pop 2
     * pop 1
     * pop +
     * add 1 2
     * 3
     *
     * (+ 1 (- 2 3))
     * push +
     * push 1
     * push R1
     * push -
     * push 2
     * push 3
     * pop 3
     * pop 2
     * pop -
     * sub 2 3
     * R1 -1
     * pop R1
     * pop 1
     * pop +
     * add 1 -1
     * 0
     */
    
    /* push function name */
    uint8_t operator;
    
    if (!strcasecmp(name, "+")) {
        operator = OP_ADD;
        debug("OP_ADD \n");
    }
    else if (!strcasecmp(name, "-")) {
        operator = OP_SUBTRACT;
        debug("OP_SUBTRACT \n");
    }
    else if (!strcasecmp(name, "*")) {
        operator = OP_MULTIPLY;
        debug("OP_MULTIPLY \n");
    }
    else if (!strcasecmp(name, "/")) {
        operator = OP_DIVIDE;
        debug("OP_DIVIDE \n");
    }
    else {
        debug_err("Unknown name:%s \n", name);
        goto FAIL;
    }
    
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
            int constant = chunk_add_constant(chunk, v);
            chunk_write(chunk, OP_CONSTANT, line);
            chunk_write(chunk, constant, line);
        }
        else if (obj_is_list(obj)) {
            
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
    
    chunk_write(chunk, operator, line);
    
    //    disassemble_chunk(chunk, "test chunk");
    
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
        
        /* TODO */
        chunk_write(&c->chunk, OP_RETURN, 666);
        
        f = f->next;
    }
    
    out(ok, COMPILER_OK);
    
FAIL:
    func_fail();
    ml_err_signal(ML_ERR_COMPILER);
    return COMPILER_ERR;
}

