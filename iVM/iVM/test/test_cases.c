

#include "test_cases.h"

#include "../debug.h"
#include "../reader.h"
#include "../gc.h"
#include "../util.h"
#include "../mem.h"

#include "../compiler.h"
#include "../disassembler.h"
#include "../vm.h"

#define SWITCH false

#define TEST_SELF_EVAL_FORM_ON (SWITCH || false)
#define TEST_CHAR_ON (SWITCH || false)
#define TEST_QUOTE_ON (SWITCH || false)
#define TEST_ATOM_ON (SWITCH || false)
#define TEST_CONS_ON (SWITCH || false)
#define TEST_CAR_ON (SWITCH || false)
#define TEST_CDR_ON (SWITCH || false)
#define TEST_CONS_CAR_CDR_ON (SWITCH || false)
#define TEST_EQ_ON (SWITCH || false)
#define TEST_LIST_ON (SWITCH || false)
#define TEST_LIST_CAR_ON (SWITCH || false)
#define TEST_LIST_CDR_ON (SWITCH || false)
#define TEST_LIST_CONS_CAR_CDR_ON (SWITCH || false)


#define TEST_SETQ_ON (SWITCH || false)
#define TEST_IF_ON (SWITCH || false)

#define TEST_PRINTER_ON (SWITCH || false)
#define TEST_NUM_FUNCALL_ON (SWITCH || false)

#define TEST_LOOP_ON (SWITCH || false)
#define TEST_DEFUN_ON (SWITCH || false)

#define TEST_VM_ON (SWITCH || true)

const char* cases[] =
{
    "test/test_vm.lisp",
    
#if 0
    "test/test.lisp",
#endif
    
#if TEST_SELF_EVAL_FORM_ON
    "test/test_self_eval_form.lisp",
#endif
    
#if TEST_CHAR_ON 
    "test/test_char.lisp",
#endif
    
#if TEST_QUOTE_ON
    "test/test_quote.lisp",
#endif
    
#if TEST_ATOM_ON 
    "test/test_atom.lisp",
#endif    
    
#if TEST_CONS_ON 
    "test/test_cons.lisp",
#endif
    
#if TEST_CAR_ON 
    "test/test_car.lisp",
#endif
    
#if TEST_CDR_ON 
    "test/test_cdr.lisp",
#endif    
    
#if TEST_CONS_CAR_CDR_ON
    "test/test_cons_car_cdr.lisp",
#endif
    
#if TEST_LIST_ON
    "test/test_list.lisp",
#endif
    
#if TEST_LIST_CAR_ON
    "test/test_list_car.lisp",
#endif
    
#if TEST_LIST_CDR_ON
    "test/test_list_cdr.lisp",
#endif
    
#if TEST_LIST_CONS_CAR_CDR_ON
    "test/test_list_cons_car_cdr.lisp",
#endif
    
#if TEST_EQ_ON
    "test/test_eq.lisp",
#endif
    
    
#if TEST_SETQ_ON
    "test/test_setq.lisp",
#endif
    
#if TEST_IF_ON
    "test/test_if.lisp",
#endif
    
    
#if TEST_PRINTER_ON
    "test/test_printer.lisp",
#endif
    
#if TEST_NUM_FUNCALL_ON
    "test/test_number_funcall.lisp",
#endif
    
#if TEST_LOOP_ON
    "test/test_loop.lisp",
#endif
    
#if TEST_DEFUN_ON
    "test/test_defun.lisp",
#endif
    
};


bool
test_case_run(void)
{
    reader_s reader;
    reader_rt_t reader_rt;
    gc_s gc;
    
    func_s();
    
    for (int i = 0; i < ARR_LEN(cases); i++) {
        
        gc = gc_new();
        if (gc.id < 0) {
            
            debug_err("create gc object failed \n");
            goto FAIL;
        }
        
        //        reader_rt = ml_reader_load_file(&reader, cases[i]);
        //        if (reader_rt != READER_OK) goto FAIL;
        
        reader_rt = ml_reader_load(&reader, cases[i]);
        if (reader_rt != READER_OK) goto FAIL;
        
        gc_show();
        gc_free();
        mm_show();
        
        static compiler_t compiler;
        compile_init(&compiler);
        compiler_ret_t compiler_rt = compile(&reader.lex.forms);
        if (compiler_rt != COMPILER_OK) goto FAIL;
        
        disassemble_chunk(&compiler.chunk, "test_chunk.vm");
        vm_interpret(&compiler.chunk);
        chunk_free(&compiler.chunk);
        
        disassemble_load("test_chunk.vm", &compiler.chunk);
        vm_interpret(&compiler.chunk);
        chunk_free(&compiler.chunk);
    }
    
    out(ok, true);
    
FAIL:
    out(fail, false);
}
