//
//  compiler.h
//  iVM
//
//  Created by TobyMars on 2021/2/16.
//  Copyright © 2021 TobyMars. All rights reserved.
//

#ifndef compiler_h
#define compiler_h

#include "common.h"

#include "chunk.h"
#include "form.h"

typedef enum {
    COMPILER_OK,
    COMPILER_ERR,
    COMPILER_ERR_NULL,
    COMPILER_UNKNOWN,
} compiler_ret_t;

typedef struct {
    chunk_t chunk;
} compiler_t;


bool compile_init(compiler_t *compiler);
compiler_ret_t compile(form_s *form);

#endif /* compiler_h */
