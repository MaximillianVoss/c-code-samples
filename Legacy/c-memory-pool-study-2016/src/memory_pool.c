#include "memory_pool.h"

#include <stdint.h>
#include <string.h>

#define MEMORY_POOL_BLOCK_MAGIC UINT32_C(0x4D504F4C)
#define MEMORY_POOL_NO_OFFSET SIZE_MAX

typedef struct block_header {
    size_t capacity;
    size_t requested_size;
    size_t previous_offset;
    size_t next_offset;
    uint32_t magic;
    unsigned char is_free;
} block_header;

static bool align_up(size_t value, size_t alignment, size_t *result)
{
    const size_t remainder = value % alignment;

    if (remainder == 0U) {
        *result = value;
        return true;
    }

    const size_t increment = alignment - remainder;
    if (value > SIZE_MAX - increment) {
        return false;
    }

    *result = value + increment;
    return true;
}

size_t memory_pool_alignment(void)
{
    return _Alignof(memory_pool_max_alignment);
}

size_t memory_pool_overhead(void)
{
    size_t result = 0U;
    (void)align_up(sizeof(block_header), memory_pool_alignment(), &result);
    return result;
}

static bool read_header(
    const memory_pool *pool,
    size_t offset,
    block_header *header)
{
    if (offset > pool->capacity ||
        sizeof(*header) > pool->capacity - offset) {
        return false;
    }

    memcpy(header, pool->storage + offset, sizeof(*header));
    return true;
}

static void write_header(
    memory_pool *pool,
    size_t offset,
    const block_header *header)
{
    memcpy(pool->storage + offset, header, sizeof(*header));
}

static bool pool_basics_are_valid(const memory_pool *pool)
{
    const size_t overhead = memory_pool_overhead();
    const size_t alignment = memory_pool_alignment();

    return pool != NULL &&
        pool->initialized &&
        pool->storage != NULL &&
        pool->capacity >= overhead + alignment &&
        pool->first_block_offset == 0U &&
        (uintptr_t)pool->storage % alignment == 0U &&
        pool->capacity % alignment == 0U;
}

bool memory_pool_init(memory_pool *pool, void *buffer, size_t buffer_size)
{
    if (pool == NULL) {
        return false;
    }

    if (buffer == NULL) {
        *pool = (memory_pool){0};
        return false;
    }

    const size_t alignment = memory_pool_alignment();
    const size_t overhead = memory_pool_overhead();
    const uintptr_t address = (uintptr_t)buffer;
    const uintptr_t pool_address = (uintptr_t)pool;
    if (buffer_size > UINTPTR_MAX - address ||
        sizeof(*pool) > UINTPTR_MAX - pool_address) {
        return false;
    }

    const uintptr_t buffer_end = address + buffer_size;
    const uintptr_t pool_end = pool_address + sizeof(*pool);
    if (pool_address < buffer_end && address < pool_end) {
        return false;
    }

    *pool = (memory_pool){0};
    const size_t leading = (alignment - (address % alignment)) % alignment;

    if (buffer_size <= leading) {
        return false;
    }

    size_t capacity = buffer_size - leading;
    capacity -= capacity % alignment;
    if (capacity < overhead + alignment) {
        return false;
    }

    pool->storage = (unsigned char *)buffer + leading;
    pool->capacity = capacity;
    pool->first_block_offset = 0U;
    pool->active_allocations = 0U;
    pool->initialized = true;

    const block_header first = {
        capacity - overhead,
        0U,
        MEMORY_POOL_NO_OFFSET,
        MEMORY_POOL_NO_OFFSET,
        MEMORY_POOL_BLOCK_MAGIC,
        1U
    };
    write_header(pool, 0U, &first);
    return true;
}

bool memory_pool_reset(memory_pool *pool)
{
    if (pool == NULL || !pool->initialized) {
        return false;
    }

    unsigned char *const storage = pool->storage;
    const size_t capacity = pool->capacity;
    return memory_pool_init(pool, storage, capacity);
}

bool memory_pool_validate(const memory_pool *pool)
{
    if (!pool_basics_are_valid(pool)) {
        return false;
    }

    const size_t overhead = memory_pool_overhead();
    const size_t alignment = memory_pool_alignment();
    size_t offset = pool->first_block_offset;
    size_t previous = MEMORY_POOL_NO_OFFSET;
    size_t allocated = 0U;
    size_t visited = 0U;
    bool previous_was_free = false;

    while (offset != MEMORY_POOL_NO_OFFSET) {
        if (++visited > pool->capacity / overhead + 1U ||
            offset % alignment != 0U) {
            return false;
        }

        block_header header;
        if (!read_header(pool, offset, &header) ||
            header.magic != MEMORY_POOL_BLOCK_MAGIC ||
            header.is_free > 1U ||
            header.previous_offset != previous ||
            header.capacity % alignment != 0U) {
            return false;
        }

        if (header.is_free != 0U) {
            if (header.requested_size != 0U || previous_was_free) {
                return false;
            }
        } else {
            if (header.requested_size == 0U ||
                header.requested_size > header.capacity) {
                return false;
            }
            ++allocated;
        }

        if (offset > pool->capacity - overhead ||
            header.capacity > pool->capacity - offset - overhead) {
            return false;
        }

        const size_t block_end = offset + overhead + header.capacity;
        if (header.next_offset == MEMORY_POOL_NO_OFFSET) {
            if (block_end != pool->capacity) {
                return false;
            }
        } else if (header.next_offset != block_end) {
            return false;
        }

        previous_was_free = header.is_free != 0U;
        previous = offset;
        offset = header.next_offset;
    }

    return allocated == pool->active_allocations;
}

bool memory_pool_contains(const memory_pool *pool, const void *pointer)
{
    if (!pool_basics_are_valid(pool) || pointer == NULL) {
        return false;
    }

    const uintptr_t begin = (uintptr_t)pool->storage;
    const uintptr_t address = (uintptr_t)pointer;
    if (pool->capacity > UINTPTR_MAX - begin) {
        return false;
    }

    return address >= begin && address < begin + pool->capacity;
}

static bool find_payload(
    const memory_pool *pool,
    const void *pointer,
    size_t *found_offset,
    block_header *found_header)
{
    const size_t overhead = memory_pool_overhead();
    size_t offset = pool->first_block_offset;

    while (offset != MEMORY_POOL_NO_OFFSET) {
        block_header header;
        if (!read_header(pool, offset, &header)) {
            return false;
        }

        if (pool->storage + offset + overhead == pointer) {
            *found_offset = offset;
            *found_header = header;
            return true;
        }

        offset = header.next_offset;
    }

    return false;
}

void *memory_pool_allocate(memory_pool *pool, size_t size)
{
    if (size == 0U || !memory_pool_validate(pool)) {
        return NULL;
    }

    const size_t alignment = memory_pool_alignment();
    const size_t overhead = memory_pool_overhead();
    size_t required = 0U;
    if (!align_up(size, alignment, &required)) {
        return NULL;
    }

    size_t offset = pool->first_block_offset;
    while (offset != MEMORY_POOL_NO_OFFSET) {
        block_header current;
        if (!read_header(pool, offset, &current)) {
            return NULL;
        }

        if (current.is_free != 0U && current.capacity >= required) {
            const size_t remainder = current.capacity - required;
            if (remainder >= overhead + alignment) {
                const size_t new_offset = offset + overhead + required;
                const block_header split = {
                    remainder - overhead,
                    0U,
                    offset,
                    current.next_offset,
                    MEMORY_POOL_BLOCK_MAGIC,
                    1U
                };

                if (current.next_offset != MEMORY_POOL_NO_OFFSET) {
                    block_header next;
                    if (!read_header(pool, current.next_offset, &next)) {
                        return NULL;
                    }
                    next.previous_offset = new_offset;
                    write_header(pool, current.next_offset, &next);
                }

                current.capacity = required;
                current.next_offset = new_offset;
                write_header(pool, new_offset, &split);
            }

            current.requested_size = size;
            current.is_free = 0U;
            write_header(pool, offset, &current);
            ++pool->active_allocations;
            return pool->storage + offset + overhead;
        }

        offset = current.next_offset;
    }

    return NULL;
}

static void merge_with_next(
    memory_pool *pool,
    size_t offset,
    block_header *current)
{
    if (current->next_offset == MEMORY_POOL_NO_OFFSET) {
        return;
    }

    block_header next;
    if (!read_header(pool, current->next_offset, &next) || next.is_free == 0U) {
        return;
    }

    current->capacity += memory_pool_overhead() + next.capacity;
    current->next_offset = next.next_offset;
    if (next.next_offset != MEMORY_POOL_NO_OFFSET) {
        block_header after;
        if (read_header(pool, next.next_offset, &after)) {
            after.previous_offset = offset;
            write_header(pool, next.next_offset, &after);
        }
    }
    write_header(pool, offset, current);
}

memory_pool_status memory_pool_deallocate(memory_pool *pool, void *pointer)
{
    if (pool == NULL || pointer == NULL) {
        return MEMORY_POOL_INVALID_ARGUMENT;
    }
    if (!pool->initialized) {
        return MEMORY_POOL_NOT_INITIALIZED;
    }
    if (!memory_pool_validate(pool)) {
        return MEMORY_POOL_CORRUPTED;
    }
    if (!memory_pool_contains(pool, pointer)) {
        return MEMORY_POOL_POINTER_OUTSIDE_POOL;
    }

    size_t offset = 0U;
    block_header current;
    if (!find_payload(pool, pointer, &offset, &current)) {
        return MEMORY_POOL_POINTER_NOT_ALLOCATED;
    }
    if (current.is_free != 0U) {
        return MEMORY_POOL_DOUBLE_FREE;
    }

    current.requested_size = 0U;
    current.is_free = 1U;
    --pool->active_allocations;
    write_header(pool, offset, &current);
    merge_with_next(pool, offset, &current);

    if (current.previous_offset != MEMORY_POOL_NO_OFFSET) {
        const size_t previous_offset = current.previous_offset;
        block_header previous;
        if (read_header(pool, previous_offset, &previous) &&
            previous.is_free != 0U) {
            merge_with_next(pool, previous_offset, &previous);
        }
    }

    return memory_pool_validate(pool) ? MEMORY_POOL_OK : MEMORY_POOL_CORRUPTED;
}

size_t memory_pool_allocation_size(
    const memory_pool *pool,
    const void *pointer)
{
    if (!memory_pool_validate(pool) || pointer == NULL) {
        return 0U;
    }

    size_t offset = 0U;
    block_header header;
    if (!find_payload(pool, pointer, &offset, &header) || header.is_free != 0U) {
        return 0U;
    }

    return header.requested_size;
}

memory_pool_stats memory_pool_get_stats(const memory_pool *pool)
{
    memory_pool_stats stats = {0};
    if (!memory_pool_validate(pool)) {
        return stats;
    }

    stats.managed_bytes = pool->capacity;
    stats.active_allocations = pool->active_allocations;

    size_t offset = pool->first_block_offset;
    while (offset != MEMORY_POOL_NO_OFFSET) {
        block_header header;
        if (!read_header(pool, offset, &header)) {
            return (memory_pool_stats){0};
        }

        ++stats.block_count;
        stats.payload_bytes += header.capacity;
        if (header.is_free != 0U) {
            stats.free_bytes += header.capacity;
            if (header.capacity > stats.largest_free_block) {
                stats.largest_free_block = header.capacity;
            }
        } else {
            stats.requested_bytes += header.requested_size;
            stats.reserved_bytes += header.capacity;
        }

        offset = header.next_offset;
    }

    return stats;
}

bool memory_pool_is_fully_free(const memory_pool *pool)
{
    if (!memory_pool_validate(pool) || pool->active_allocations != 0U) {
        return false;
    }

    block_header first;
    return read_header(pool, pool->first_block_offset, &first) &&
        first.is_free != 0U &&
        first.next_offset == MEMORY_POOL_NO_OFFSET;
}
