
#ifndef memory_h
#define memory_h

#include "common.h"

#define MEM_GROW_CAPACITY(capacity) \
((capacity) < 8 ? 8 : (capacity) * 2)

#define MEM_GROW_ARRAY(type, pointer, old_count, new_count) \
(type*)mem_reallocate(pointer, sizeof(type) * (old_count),	\
sizeof(type) * (new_count))

#define MEM_FREE_ARRAY(type, pointer, old_count) \
mem_reallocate(pointer, sizeof(type) * (old_count), 0)

void* mem_reallocate(void* pointer, size_t old_size, size_t new_size);


#endif /* memory_h */
