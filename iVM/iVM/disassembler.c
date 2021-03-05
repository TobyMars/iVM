
#include "disassembler.h"

#include "debug.h"
#include "util.h"
#include "log.h"

static const char *m_fname = NULL;

#undef debug
#define debug(...) ;

bool
disassemble_load(const char *fname, chunk_t *chunk)
{
    file_info finfo;
    memset(&finfo, 0, sizeof(finfo));
    finfo.f_name = fname;
    if (!log_fread(&finfo)) {
        return false;
    }
    
    char buf[512];
    int cnt;
    char *s;
    size_t len;
    int offset = 0;
    
    s = finfo.f_buf;
    len = finfo.buf_sz;
    ml_util_show_buf(s, len);
    
    /* get file name
     */
    cnt = sscanf(finfo.f_buf, "== %s ==\n", buf);
    if (cnt == 1) {
        debug("file name: %s \n", buf);
    }
    offset = sprintf(buf, "== %s ==\n", buf);
    s += offset;
    len -= offset;
    ml_util_show_buf(s, len);
    
    /* get registers
     */
    reg_t reg = 0;
    reg_array_t *array = &chunk->reg_array;
    cnt = sscanf(s, "registers: [ 0x%08x ", &reg);
    if (cnt != 1) {
        goto FAIL;
    }
    offset = sprintf(buf, "registers: [ 0x%08x ", reg);
    reg_write(array, reg);
    reg_print(reg);
    s += offset;
    len -= offset;
    ml_util_show_buf(s, len);
    while (1) {
        cnt = sscanf(s, "0x%08x ", &reg);
        if (cnt != 1) {
            if (memcmp(s, "]\n", 2)) {
                goto FAIL;
            }
            offset = strlen("]\n");
            s += offset;
            len -= offset;
            ml_util_show_buf(s, len);
            break;
        }
        
        offset = sprintf(buf, "0x%08x ", reg);
        reg_write(array, reg);
        reg_print(reg);
        s += offset;
        len -= offset;
        ml_util_show_buf(s, len);
    }
    
    /* get instruction count
     */
    int count = 0;
    cnt = sscanf(s, "instruction count:%08d\n", &count);
    if (cnt != 1) {
        goto FAIL;
    }
    offset = sprintf(buf, "instruction count:%08d\n", count);
    s += offset;
    len -= offset;
    ml_util_show_buf(s, len);
    
    int addr = 0;
    int line = 0;
    char operator[32];
    instruction_t instruction = 0;
    while (len > 0) {
        memset(operator, 0, sizeof(operator));
        
        cnt = sscanf(s, "%04x:%08x %4d %s\n",
                     &addr, &instruction, &line, operator);
        if (cnt == 4) {
            debug("%04x:%08x %4d %s\n", addr, instruction, line, operator);
            offset = sprintf(buf, "%04x:%08x %4d %s\n", addr, instruction, line, operator);
        }
        else {
            debug_err("unknown codes: \n");
            ml_util_show_buf(s, len);
            break;
        }
        
        chunk_write(chunk, instruction, line);
        
        s += offset;
        len -= offset;
        if (len > 0) {
            ml_util_show_buf(s, len);
        }
    }
    
    disassemble_chunk(chunk, fname);
    
    fe();
    return true;
    
FAIL:
    return false;
}

static int
dis_simple_instruction(const char *name, int offset)
{
    debug("%s\n", name);
    log_file(m_fname, "%s\n", name);
    
    return (offset + 1);
}

static int
dis_constant_instruction(const char *name, chunk_t *chunk, int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    debug("%-16s %4d '", name, constant);
    log_file(m_fname, "%-16s %4d '", name, constant);
    //    value_print(chunk->constants.values[constant]);
    log_file(m_fname, "%g", chunk->constants.values[constant]);
    debug("'\n");
    log_file(m_fname, "'\n");
    
    return (offset + 2);
}

static void
log_registers(reg_array_t *array)
{
    log_file(m_fname, "registers: [ ");
    for (int i = 0; i < array->count; i++) {
        log_file(m_fname, "0x%08x ", array->regs[i]);
    }
    log_file(m_fname, "]\n");
}

void
disassemble_chunk(chunk_t *chunk, const char *name)
{
    remove(name);
    
    m_fname = name;
    
    debug("== %s ==\n", m_fname);
    log_file(m_fname, "== %s ==\n", m_fname);
    
    //    reg_print_all(&chunk->reg_array);
    log_registers(&chunk->reg_array);
    
    debug("instruction count:%08d\n", chunk->count);
    log_file(m_fname, "instruction count:%08d\n", chunk->count);
    
    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
    
    m_fname = NULL;
}

int
disassemble_instruction(chunk_t *chunk, int offset)
{
    int i = offset;
    instruction_t instruction = chunk->code[i];
    
    debug("%04x:%08x ", i * sizeof(instruction_t), instruction);
    log_file(m_fname, "%04x:%08x ", i * sizeof(instruction_t), instruction);
    
    debug("%4d ", chunk->lines[i]);
    log_file(m_fname, "%4d ", chunk->lines[i]);
    
    opcode_t opcode = (instruction >> 24) & 0xFF;
    switch (opcode) {
            
        case OP_CONSTANT:
            return dis_constant_instruction("OP_CONSTANT", chunk, i);
            
        case OP_NEGATE:
            return dis_simple_instruction("OP_NEGATE", i);
        case OP_ADD:
            return dis_simple_instruction("OP_ADD", i);
        case OP_SUBTRACT:
            return dis_simple_instruction("OP_SUBTRACT", i);
        case OP_MULTIPLY:
            return dis_simple_instruction("OP_MULTIPLY", i);
        case OP_DIVIDE:
            return dis_simple_instruction("OP_DIVIDE", i);
            
        case OP_RETURN:
            return dis_simple_instruction("OP_RETURN", i);
            
        default:
            debug_err(">> unknown instruction: 0x%08x \n", instruction);
            return (i + 1);
    }
}

void
disassemble_test(void)
{
    fs();
    
    chunk_t chunk;
    
    chunk_init(&chunk);
    
    int constant = chunk_add_constant(&chunk, 1.2);
    chunk_write(&chunk, OP_CONSTANT, 123);
    chunk_write(&chunk, constant, 123);
    
    chunk_write(&chunk, OP_RETURN, 123);
    
    disassemble_chunk(&chunk, "test chunk");
    
    chunk_free(&chunk);
}
