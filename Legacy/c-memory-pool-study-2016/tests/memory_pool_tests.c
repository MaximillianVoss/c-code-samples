#include "memory_pool.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "check failed at %s:%d: %s\n", \
                __FILE__, __LINE__, #condition); \
            return false; \
        } \
    } while (0)

typedef union aligned_storage {
    memory_pool_max_alignment alignment;
    unsigned char bytes[4096];
} aligned_storage;

static bool test_initialization(void)
{
    aligned_storage storage = {0};
    memory_pool pool = {0};

    CHECK(!memory_pool_init(NULL, storage.bytes, sizeof(storage.bytes)));
    CHECK(!memory_pool_init(&pool, NULL, sizeof(storage.bytes)));
    CHECK(!memory_pool_init(&pool, storage.bytes, memory_pool_overhead()));
    CHECK(memory_pool_init(&pool, storage.bytes, sizeof(storage.bytes)));
    CHECK(memory_pool_validate(&pool));
    CHECK(memory_pool_is_fully_free(&pool));
    CHECK(memory_pool_allocate(&pool, 0U) == NULL);
    CHECK(memory_pool_get_stats(&(memory_pool){0}).managed_bytes == 0U);

    const memory_pool_stats stats = memory_pool_get_stats(&pool);
    CHECK(stats.managed_bytes == sizeof(storage.bytes));
    CHECK(stats.block_count == 1U);
    CHECK(stats.active_allocations == 0U);
    CHECK(stats.free_bytes > 0U);
    CHECK(stats.free_bytes == stats.largest_free_block);
    return true;
}

static bool test_alignment_and_data(void)
{
    aligned_storage storage = {0};
    memory_pool pool = {0};
    CHECK(memory_pool_init(&pool, storage.bytes + 1, sizeof(storage.bytes) - 1U));

    unsigned char *first = memory_pool_allocate(&pool, 13U);
    double *second = memory_pool_allocate(&pool, sizeof(double) * 3U);
    CHECK(first != NULL);
    CHECK(second != NULL);
    CHECK((uintptr_t)first % memory_pool_alignment() == 0U);
    CHECK((uintptr_t)second % memory_pool_alignment() == 0U);
    CHECK(memory_pool_allocation_size(&pool, first) == 13U);
    CHECK(memory_pool_allocation_size(&pool, second) == sizeof(double) * 3U);
    CHECK(memory_pool_contains(&pool, first));
    CHECK(memory_pool_contains(&pool, second));

    memset(first, 0xA5, 13U);
    second[0] = 1.25;
    second[1] = -7.5;
    second[2] = 42.0;
    CHECK(first[12] == 0xA5U);
    CHECK(second[0] == 1.25 && second[1] == -7.5 && second[2] == 42.0);
    CHECK(memory_pool_validate(&pool));

    const memory_pool_stats stats = memory_pool_get_stats(&pool);
    CHECK(stats.active_allocations == 2U);
    CHECK(stats.requested_bytes == 13U + sizeof(double) * 3U);
    CHECK(stats.reserved_bytes >= stats.requested_bytes);
    CHECK(stats.free_bytes > 0U);
    return true;
}

static bool test_first_fit_reuses_a_hole(void)
{
    aligned_storage storage = {0};
    memory_pool pool = {0};
    CHECK(memory_pool_init(&pool, storage.bytes, sizeof(storage.bytes)));

    void *first = memory_pool_allocate(&pool, 48U);
    void *middle = memory_pool_allocate(&pool, 48U);
    void *last = memory_pool_allocate(&pool, 48U);
    CHECK(first != NULL && middle != NULL && last != NULL);
    CHECK(memory_pool_deallocate(&pool, middle) == MEMORY_POOL_OK);

    void *replacement = memory_pool_allocate(&pool, 24U);
    CHECK(replacement == middle);
    CHECK(memory_pool_validate(&pool));
    return true;
}

static bool test_coalescing(void)
{
    aligned_storage storage = {0};
    memory_pool pool = {0};
    CHECK(memory_pool_init(&pool, storage.bytes, sizeof(storage.bytes)));

    void *first = memory_pool_allocate(&pool, 64U);
    void *middle = memory_pool_allocate(&pool, 80U);
    void *last = memory_pool_allocate(&pool, 96U);
    CHECK(first != NULL && middle != NULL && last != NULL);
    CHECK(memory_pool_deallocate(&pool, middle) == MEMORY_POOL_OK);
    CHECK(memory_pool_deallocate(&pool, first) == MEMORY_POOL_OK);
    CHECK(memory_pool_deallocate(&pool, last) == MEMORY_POOL_OK);
    CHECK(memory_pool_is_fully_free(&pool));
    CHECK(memory_pool_get_stats(&pool).block_count == 1U);
    return true;
}

static bool test_exhaustion_and_recovery(void)
{
    aligned_storage storage = {0};
    memory_pool pool = {0};
    void *blocks[64] = {0};
    size_t count = 0U;
    CHECK(memory_pool_init(&pool, storage.bytes, sizeof(storage.bytes)));

    while (count < 64U) {
        blocks[count] = memory_pool_allocate(&pool, 64U);
        if (blocks[count] == NULL) {
            break;
        }
        ++count;
    }

    CHECK(count > 2U);
    CHECK(memory_pool_allocate(&pool, 64U) == NULL);
    while (count > 0U) {
        --count;
        CHECK(memory_pool_deallocate(&pool, blocks[count]) == MEMORY_POOL_OK);
    }
    CHECK(memory_pool_is_fully_free(&pool));
    return true;
}

static bool test_invalid_and_double_free(void)
{
    aligned_storage storage = {0};
    memory_pool pool = {0};
    int outside = 0;
    CHECK(memory_pool_init(&pool, storage.bytes, sizeof(storage.bytes)));

    unsigned char *pointer = memory_pool_allocate(&pool, 32U);
    CHECK(pointer != NULL);
    CHECK(memory_pool_deallocate(&pool, &outside) ==
        MEMORY_POOL_POINTER_OUTSIDE_POOL);
    CHECK(memory_pool_deallocate(&pool, pointer + 1) ==
        MEMORY_POOL_POINTER_NOT_ALLOCATED);
    CHECK(memory_pool_deallocate(&pool, NULL) == MEMORY_POOL_INVALID_ARGUMENT);
    CHECK(memory_pool_deallocate(&pool, pointer) == MEMORY_POOL_OK);
    CHECK(memory_pool_deallocate(&pool, pointer) == MEMORY_POOL_DOUBLE_FREE);
    CHECK(memory_pool_validate(&pool));
    return true;
}

static bool test_overflow_and_reset(void)
{
    aligned_storage storage = {0};
    memory_pool pool = {0};
    CHECK(memory_pool_init(&pool, storage.bytes, sizeof(storage.bytes)));
    CHECK(memory_pool_allocate(&pool, SIZE_MAX) == NULL);

    void *pointer = memory_pool_allocate(&pool, 100U);
    CHECK(pointer != NULL);
    CHECK(!memory_pool_is_fully_free(&pool));
    CHECK(memory_pool_reset(&pool));
    CHECK(memory_pool_is_fully_free(&pool));
    CHECK(memory_pool_allocation_size(&pool, pointer) == 0U);
    return true;
}

int main(void)
{
    typedef bool (*test_function)(void);
    const test_function tests[] = {
        test_initialization,
        test_alignment_and_data,
        test_first_fit_reuses_a_hole,
        test_coalescing,
        test_exhaustion_and_recovery,
        test_invalid_and_double_free,
        test_overflow_and_reset
    };
    const char *const names[] = {
        "initialization",
        "alignment and data",
        "first-fit reuse",
        "coalescing",
        "exhaustion and recovery",
        "invalid and double free",
        "overflow and reset"
    };

    for (size_t index = 0U; index < sizeof(tests) / sizeof(tests[0]); ++index) {
        if (!tests[index]()) {
            fprintf(stderr, "[FAIL] %s\n", names[index]);
            return 1;
        }
        printf("[PASS] %s\n", names[index]);
    }

    return 0;
}
