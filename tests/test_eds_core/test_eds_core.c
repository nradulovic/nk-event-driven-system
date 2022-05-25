/*
 * test_nk_array.cpp
 *
 *  Created on: Feb 21, 2021
 *      Author: nenad
 */

#include "../nk_test.h"

#include "fff.h"
#include "eds/source/sys/eds_core.h"
#include "eds/source/eds_object.h"

DEFINE_FFF_GLOBALS

static void
test__eds_core__vector_init(void)
{
#define DUMMY_VECTOR_SIZE 10
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    /* CUT */
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);

    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(0u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_insert__first(void)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert = DUMMY_INSERT_VALUE;

    /* CUT */
    eds_core__vector_insert(&vector, 0, &dummy_insert);

    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(1u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
    NK_TEST__EXPECT_UINT(DUMMY_INSERT_VALUE);
    NK_TEST__ACTUAL_UINT(*(uint32_t* )eds_core__vector_peek(&vector, 0));
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_insert__second(void)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
    uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
    eds_core__vector_insert(&vector, 0, &dummy_insert_1);
    /* CUT */
    eds_core__vector_insert(&vector, 1, &dummy_insert_2);

    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(2u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
    NK_TEST__EXPECT_UINT(DUMMY_INSERT_1_VALUE);
    NK_TEST__ACTUAL_UINT(*(uint32_t* )eds_core__vector_peek(&vector, 0));
    NK_TEST__EXPECT_UINT(DUMMY_INSERT_2_VALUE);
    NK_TEST__ACTUAL_UINT(*(uint32_t* )eds_core__vector_peek(&vector, 1));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_remove__first(void)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert = DUMMY_INSERT_VALUE;
    eds_core__vector_insert(&vector, 0, &dummy_insert);

    /* CUT */
    eds_core__vector_remove(&vector, 0);
    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(0u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_remove__second_of_two(void)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
    uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
    eds_core__vector_insert(&vector, 0, &dummy_insert_1);
    eds_core__vector_insert(&vector, 1, &dummy_insert_2);
    /* CUT */
    eds_core__vector_remove(&vector, 1);

    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(1u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
    NK_TEST__EXPECT_UINT(DUMMY_INSERT_1_VALUE);
    NK_TEST__ACTUAL_UINT(*(uint32_t* )eds_core__vector_peek(&vector, 0));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_remove__first_of_two(void)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
    uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
    eds_core__vector_insert(&vector, 0, &dummy_insert_1);
    eds_core__vector_insert(&vector, 1, &dummy_insert_2);
    /* CUT */
    eds_core__vector_remove(&vector, 0);

    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(1u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
    NK_TEST__EXPECT_UINT(DUMMY_INSERT_2_VALUE);
    NK_TEST__ACTUAL_UINT(*(uint32_t* )eds_core__vector_peek(&vector, 0));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_remove__all(void)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
    uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
    eds_core__vector_insert(&vector, 0, &dummy_insert_1);
    eds_core__vector_insert(&vector, 1, &dummy_insert_2);
    /* CUT */
    eds_core__vector_remove(&vector, 0);
    eds_core__vector_remove(&vector, 0);

    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(0u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_insert__second_at_zero(void)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
    uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
    eds_core__vector_insert(&vector, 0, &dummy_insert_1);
    /* CUT */
    eds_core__vector_insert(&vector, 0, &dummy_insert_2);

    NK_TEST__EXPECT_UINT(DUMMY_VECTOR_SIZE);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_size(&vector));
    NK_TEST__EXPECT_UINT(2u);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries(&vector));
    NK_TEST__EXPECT_BOOL(false);
    NK_TEST__ACTUAL_BOOL(eds_core__vector_is_full(&vector));
    NK_TEST__EXPECT_UINT(DUMMY_INSERT_2_VALUE);
    NK_TEST__ACTUAL_UINT(*(uint32_t* )eds_core__vector_peek(&vector, 0));
    NK_TEST__EXPECT_UINT(DUMMY_INSERT_1_VALUE);
    NK_TEST__ACTUAL_UINT(*(uint32_t* )eds_core__vector_peek(&vector, 1));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__vector_is_full(void)
{
#define DUMMY_VECTOR_SIZE 2
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
    struct eds_object__vector vector;
    uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
    eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
    uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
    uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
    eds_core__vector_insert(&vector, 0, &dummy_insert_1);
    eds_core__vector_insert(&vector, 0, &dummy_insert_2);

    /* CUT */
    bool is_full = eds_core__vector_is_full(&vector);

    NK_TEST__EXPECT_BOOL(true);
    NK_TEST__ACTUAL_BOOL(is_full);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

static void
test__eds_core__insert_leak(void)
{
#define DUMMY_VECTOR_SIZE 2
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
#define BEGIN_GUARD_VALUE 0xdeadbeef
#define END_GUARD_VALUE 0xdeadbebe

    struct eds_object__vector vector;
    struct dummy_buffer_with_guards
    {
        uint32_t begin_guard;
        uint32_t entries[DUMMY_VECTOR_SIZE];
        uint32_t end_guard;
    } dummy_entries =
        {
        .begin_guard = BEGIN_GUARD_VALUE, .end_guard = END_GUARD_VALUE
        };
    uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
    uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
    eds_core__vector_init(&vector, &dummy_entries.entries[0], sizeof(dummy_entries.entries[0]),
    DUMMY_VECTOR_SIZE);
    eds_core__vector_insert(&vector, 0, &dummy_insert_1);
    eds_core__vector_insert(&vector, 0, &dummy_insert_2);
    /* CUT */
    bool is_full = eds_core__vector_is_full(&vector);

    NK_TEST__EXPECT_UINT(BEGIN_GUARD_VALUE);
    NK_TEST__ACTUAL_UINT(dummy_entries.begin_guard);
    NK_TEST__EXPECT_UINT(END_GUARD_VALUE);
    NK_TEST__ACTUAL_UINT(dummy_entries.end_guard);
    NK_TEST__EXPECT_BOOL(true);
    NK_TEST__ACTUAL_BOOL(is_full);
#undef END_GUARD_VALUE
#undef BEGIN_GUARD_VALUE
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

void
test_execute(void)
{
    static const struct nk_testsuite__test tests[] =
        {
        NK_TEST__TEST(test__eds_core__vector_init),
    NK_TEST__TEST(test__eds_core__vector_insert__first),
NK_TEST__TEST(test__eds_core__vector_insert__second),
NK_TEST__TEST(test__eds_core__vector_remove__first),
NK_TEST__TEST(test__eds_core__vector_remove__second_of_two),
NK_TEST__TEST(test__eds_core__vector_remove__first_of_two),
NK_TEST__TEST(test__eds_core__vector_remove__all),
NK_TEST__TEST(test__eds_core__vector_insert__second_at_zero),
NK_TEST__TEST(test__eds_core__vector_is_full),
NK_TEST__TEST(test__eds_core__insert_leak),
NK_TEST__TEST_TERMINATE()
}
    ;
    nk_test__run_fixture(tests, NULL, NULL, NK_TESTSUITE__FIXTURE_NAME(none));
}
