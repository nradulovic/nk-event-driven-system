/*
 * test_nk_array.cpp
 *
 *  Created on: Feb 21, 2021
 *      Author: nenad
 */

#include "../nk_test.h"

#include "fff.h"
#include "eds/source/eds.h"
#include "eds/source/eds_object.h"

DEFINE_FFF_GLOBALS

FAKE_VALUE_FUNC(bool, eds_core__vector_is_full, const struct eds_object__vector *)
FAKE_VALUE_FUNC(struct eds_object__mem *, eds_mem__find, const struct eds_object__vector *, size_t)
FAKE_VOID_FUNC(eds_mem__init,
    struct eds_object__mem *,
    eds__mem_alloc_fn *,
    eds__mem_dealloc_fn *,
    void *,
    size_t)
FAKE_VOID_FUNC(eds_mem__add, struct eds_object__vector *, const struct eds_object__mem *)

FAKE_VALUE_FUNC(void *, fake_alloc, void*, size_t)
FAKE_VOID_FUNC(fake_dealloc, void*, void*)

struct eds_object__vector mem__instances;

static void
test__mem_add_allocator__alloc_null(void)
{
    /* CUT */
    eds__error error = eds__mem_add_allocator(NULL, NULL, NULL, 0);

    NK_TEST__EQUAL_UINT(EDS__ERROR_INVALID_ARGUMENT, error);
    NK_TEST__EQUAL_UINT(eds_core__vector_is_full_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__find_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__init_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__add_fake.call_count, 0);
}

static void
test__mem_add_allocator__dealloc_null(void)
{
    /* CUT */
    eds__error error = eds__mem_add_allocator(fake_alloc, NULL, NULL, 0);
    NK_TEST__EQUAL_UINT(EDS__ERROR_INVALID_ARGUMENT, error);
    NK_TEST__EQUAL_UINT(eds_core__vector_is_full_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__find_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__init_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__add_fake.call_count, 0);
}

static void
test__mem_add_allocator__size_zero(void)
{
    /* CUT */
    eds__error error = eds__mem_add_allocator(fake_alloc, fake_dealloc, NULL, 0);

    NK_TEST__EQUAL_UINT(EDS__ERROR_INVALID_ARGUMENT, error);
    NK_TEST__EQUAL_UINT(eds_core__vector_is_full_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__find_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__init_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__add_fake.call_count, 0);
}

static void
test__mem_add_allocator__size_below_min(void)
{
    /* CUT */
    eds__error error = eds__mem_add_allocator(fake_alloc,
        fake_dealloc,
        NULL,
        (EDS__DEFAULT_MEM_MIN_BYTES - 1u));

    NK_TEST__EQUAL_UINT(EDS__ERROR_INVALID_ARGUMENT, error);
    NK_TEST__EQUAL_UINT(eds_core__vector_is_full_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__find_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__init_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__add_fake.call_count, 0);
}

static void
test__mem_add_allocator__no_resource(void)
{
    /* Setup that vector holding memory allocators is full, so we are forbidden to add more */
    eds_core__vector_is_full_fake.return_val = true;

    /* CUT */
    eds__error error = eds__mem_add_allocator(fake_alloc,
        fake_dealloc,
        NULL,
        EDS__DEFAULT_MEM_MIN_BYTES);

    NK_TEST__EQUAL_UINT(EDS__ERROR_NO_RESOURCE, error);
    NK_TEST__EQUAL_UINT(eds_core__vector_is_full_fake.call_count, 1);
    NK_TEST__EQUAL_UINT(eds_mem__find_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__init_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__add_fake.call_count, 0);
}

static void
test__mem_add_allocator__already_exists(void)
{
    /* Setup that vector holding memory allocators is not full but already has the allocator with
     * the same instance size, so we are forbidden add the current allocator */
    struct eds_object__mem fake_mem;

    eds_mem__find_fake.return_val = &fake_mem;

    /* CUT */
    eds__error error = eds__mem_add_allocator(fake_alloc,
        fake_dealloc,
        NULL,
        EDS__DEFAULT_MEM_MIN_BYTES);

    NK_TEST__EQUAL_UINT(EDS__ERROR_ALREADY_EXISTS, error);
    NK_TEST__EQUAL_UINT(eds_core__vector_is_full_fake.call_count, 1);
    NK_TEST__EQUAL_UINT(eds_mem__find_fake.call_count, 1);
    NK_TEST__EQUAL_SIZE(eds_mem__find_fake.arg1_history[0], EDS__DEFAULT_MEM_MIN_BYTES);
    NK_TEST__EQUAL_UINT(eds_mem__init_fake.call_count, 0);
    NK_TEST__EQUAL_UINT(eds_mem__add_fake.call_count, 0);
}

static void
test__mem_add_allocator__no_error(void)
{
    /* CUT */
    eds__error error = eds__mem_add_allocator(fake_alloc,
        fake_dealloc,
        NULL,
        EDS__DEFAULT_MEM_MIN_BYTES);

    NK_TEST__EQUAL_UINT(EDS__ERROR_NONE, error);
    NK_TEST__EQUAL_UINT(eds_core__vector_is_full_fake.call_count, 1);
    NK_TEST__EQUAL_UINT(eds_mem__find_fake.call_count, 1);
    NK_TEST__EQUAL_SIZE(eds_mem__find_fake.arg1_history[0], EDS__DEFAULT_MEM_MIN_BYTES);
    NK_TEST__EQUAL_UINT(eds_mem__init_fake.call_count, 1);
    NK_TEST__EQUAL_UINT(eds_mem__add_fake.call_count, 1);
}

static void
setup(void)
{
    RESET_FAKE(eds_core__vector_is_full);
    RESET_FAKE(eds_mem__find);
    RESET_FAKE(eds_mem__init);
    RESET_FAKE(eds_mem__add);
}

void
test_execute(void)
{
    static const struct nk_testsuite__test tests[] =
        {
        NK_TEST__TEST(test__mem_add_allocator__alloc_null),
    NK_TEST__TEST(test__mem_add_allocator__dealloc_null),
NK_TEST__TEST(test__mem_add_allocator__size_zero),
NK_TEST__TEST(test__mem_add_allocator__size_below_min),
NK_TEST__TEST(test__mem_add_allocator__no_resource),
NK_TEST__TEST(test__mem_add_allocator__already_exists),
NK_TEST__TEST(test__mem_add_allocator__no_error),
NK_TEST__TEST_TERMINATE()
}
    ;
    nk_test__run_fixture(tests, setup, NULL, NK_TESTSUITE__FIXTURE_NAME(none));
}
