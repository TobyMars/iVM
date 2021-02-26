
#include "common.h"

#include "config.h"

typedef struct {
    const char *f_name;
    FILE *f;
    long f_sz;
    char *f_buf;
    size_t buf_sz;
    char *buf_e;
} file_info;

bool log_fread(file_info *fi);

void log_file(const char *fname, const char *format, ...);


