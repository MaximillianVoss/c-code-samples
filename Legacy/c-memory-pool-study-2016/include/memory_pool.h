#ifndef C_MEMORY_POOL_H
#define C_MEMORY_POOL_H

#include <stdbool.h>
#include <stddef.h>

typedef enum memory_pool_status {
    MEMORY_POOL_OK = 0,
    MEMORY_POOL_INVALID_ARGUMENT,
    MEMORY_POOL_NOT_INITIALIZED,
    MEMORY_POOL_POINTER_OUTSIDE_POOL,
    MEMORY_POOL_POINTER_NOT_ALLOCATED,
    MEMORY_POOL_DOUBLE_FREE,
    MEMORY_POOL_CORRUPTED
} memory_pool_status;

typedef union memory_pool_max_alignment {
    long double long_double_value;
    void *pointer_value;
    long long integer_value;
    double double_value;
} memory_pool_max_alignment;

typedef struct memory_pool {
    unsigned char *storage;
    size_t capacity;
    size_t first_block_offset;
    size_t active_allocations;
    bool initialized;
} memory_pool;

typedef struct memory_pool_stats {
    size_t managed_bytes;
    size_t payload_bytes;
    size_t requested_bytes;
    size_t reserved_bytes;
    size_t free_bytes;
    size_t largest_free_block;
    size_t block_count;
    size_t active_allocations;
} memory_pool_stats;

bool memory_pool_init(memory_pool *pool, void *buffer, size_t buffer_size);
bool memory_pool_reset(memory_pool *pool);

void *memory_pool_allocate(memory_pool *pool, size_t size);
memory_pool_status memory_pool_deallocate(memory_pool *pool, void *pointer);

bool memory_pool_validate(const memory_pool *pool);
bool memory_pool_contains(const memory_pool *pool, const void *pointer);
bool memory_pool_is_fully_free(const memory_pool *pool);

size_t memory_pool_allocation_size(const memory_pool *pool, const void *pointer);
size_t memory_pool_alignment(void);
size_t memory_pool_overhead(void);
memory_pool_stats memory_pool_get_stats(const memory_pool *pool);

#endif
