
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
    
    ml_util_show_buf(finfo.f_buf, finfo.buf_sz);
    
    /* get file name */
    cnt = sscanf(finfo.f_buf, "== %s ==\r\n", buf);
    if (cnt == 1) {
        debug("file name: %s \n", buf);
    }
    
    s = finfo.f_buf + strlen(buf) + 7;
    len = finfo.buf_sz - (strlen(buf) + 7);
    ml_util_show_buf(s, len);
    
    int addr = 0;
    int line = 0;
    char operator[32];
    int constant_index = 0;
    float val = 0;
    int offset = 0;
    while (len > 0) {
        memset(operator, 0, sizeof(operator));
        
        cnt = sscanf(s, "%04d %4d %16s %4d '%g'\n",
                     &addr, &line, operator, &constant_index, &val);
        if (cnt == 5) {
            debug("%04d %4d %-16s %4d '%g'\n", addr, line, operator, constant_index, val);
            
            offset = sprintf(buf, "%04d %4d %-16s %4d '%g'\n",
                             addr, line, operator, constant_index, val);
            //            debug("offset:%d \n", offset);
        }
        else {
            debug("%04d %4d %s\n", addr, line, operator);
            cnt = sscanf(s, "%04d %4d %s\n", &addr, &line, operator);
            offset = sprintf(buf, "%04d %4d %s\n", addr, line, operator);
            //            debug("offset:%d \n", offset);
        }
        
        uint8_t op = OP_UNKNOWN;
        if (!strcmp(operator, "OP_CONSTANT")) {
            op = OP_CONSTANT;
            int index = chunk_add_constant(chunk, val);
            chunk_write(chunk, OP_CONSTANT, line);
            chunk_write(chunk, index, line);
            
        }
        else if (!strcmp(operator, "OP_NEGATE")) {
            op = OP_NEGATE;
            chunk_write(chunk, op, line);
        }
        else if (!strcmp(operator, "OP_ADD")) {
            op = OP_ADD;
            chunk_write(chunk, op, line);
        }
        else if (!strcmp(operator, "OP_MULTIPLY")) {
            op = OP_MULTIPLY;
            chunk_write(chunk, op, line);
        }
        else if (!strcmp(operator, "OP_SUBTRACT")) {
            op = OP_SUBTRACT;
            chunk_write(chunk, op, line);
        }
        else if (!strcmp(operator, "OP_DIVIDE")) {
            op = OP_DIVIDE;
            chunk_write(chunk, op, line);
        }
        else if (!strcmp(operator, "OP_RETURN")) {
            op = OP_RETURN;
            chunk_write(chunk, op, line);
        }
        else {
            debug_err("OP_UNKNOWN:%s \n", operator);
        }
        
        s += offset;
        len -= offset;
        ml_util_show_buf(s, len);
    }
    
    disassemble_chunk(chunk, fname);
    
    fe();
    return true;
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

void
disassemble_chunk(chunk_t *chunk, const char *name)
{
    remove(name);
    
    m_fname = name;
    
    debug("== %s ==\n", m_fname);
    log_file(m_fname, "== %s ==\n", m_fname);
    
    for (int offset = 0; offset < chunk->count;) {
        offset = disassemble_instruction(chunk, offset);
    }
    
    m_fname = NULL;
}

int
disassemble_instruction(chunk_t *chunk, int offset)
{
    int i = offset;
    
    debug("%04d ", i);
    log_file(m_fname, "%04d ", i);
    
    //    if (offset > 0 && chunk->lines[i] == chunk->lines[i - 1]) {
    //        debug("   | ");
    //        log_file(m_fname, "   | ");
    //    }
    //    else {
    //        debug("%4d ", chunk->lines[i]);
    //        log_file(m_fname, "%4d ", chunk->lines[i]);
    //    }
    debug("%4d ", chunk->lines[i]);
    log_file(m_fname, "%4d ", chunk->lines[i]);
    
    uint8_t instruction = chunk->code[i];
    switch (instruction) {
            
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
