
#include "log.h"

#include "debug.h"

#include "error.h"

#include "mem.h"

static long
get_file_size(FILE *f)
{
    long cur;
    long size;
    
    cur = ftell(f);
    fseek(f, 0L, SEEK_END);
    size = ftell(f);
    fseek(f, cur, SEEK_SET);
    
    return size;
}

bool
log_fread(file_info *fi)
{
    fs();
    
    fi->f = fopen(fi->f_name, "rb");
    if (!fi->f) {
        debug_err("open file %s failed.\n", fi->f_name);
        return false;
    }
    
    fi->f_sz = get_file_size(fi->f);
    debug("file %s opened, size %lu bytes.\n", fi->f_name, fi->f_sz);
    
    fi->f_buf = (char*)mm_malloc(fi->f_sz);
    if (!fi->f_buf) return false;
    
    fi->buf_sz = fread(fi->f_buf, 1, fi->f_sz, fi->f);
    if (fi->buf_sz < fi->f_sz) {
        mm_free(fi->f_buf);
        fi->f_buf = NULL;
        fclose(fi->f);
        fi->f = NULL;
        return false;
    }
    
    fi->buf_e = fi->f_buf + fi->buf_sz - 1;
    
    fclose(fi->f);
    fi->f = NULL;
    
    fe();
    return true;
}

static bool
log_fwrite(char *name, char *line)
{
    if (!name) return false;
    
    FILE *f;
    int rt;
    
    f = fopen(name, "a");
    if (!f) return false;
    
    
    rt = fwrite(line, strlen(line), 1, f);
    if (!rt)  {
        debug_err("write [%s] failed. \n", line);
        return false;
    }
    
    fclose(f);
    
    return true;
}

void
log_file(const char *fname, const char *format, ...)
{
    va_list argp;
    
    char buf[512];
    unsigned long len;
    
    memset(buf, 0, sizeof(buf));
    
    va_start(argp, format);
    
    len = vsprintf(buf, format, argp);
    
    if (len > sizeof(buf)) {
        
        va_end(argp);
        
        debug_err("stack buffer overflow in %s \n", __func__);
        
        return;
    }
    
    va_end(argp);
    
    log_fwrite(fname, buf);
    debug("%s", buf);
}

