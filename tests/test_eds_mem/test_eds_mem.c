/*
 * test_nk_array.cpp
 *
 *  Created on: Feb 21, 2021
 *      Author: nenad
 */

#include "../nk_test.h"

#include "fff.h"
#include "eds/source/sys/eds_mem.h"
#include "eds/source/eds_object.h"

DEFINE_FFF_GLOBALS

FAKE_VALUE_FUNC(uint32_t, eds_core__vector_n_entries, const struct eds_object__vector *)
FAKE_VALUE_FUNC(void *, eds_core__vector_peek, const struct eds_object__vector *, uint32_t)
FAKE_VOID_FUNC(eds_core__vector_insert, struct eds_object__vector *, uint32_t, const void *)

FAKE_VALUE_FUNC(void *, fake_alloc, void*, size_t)
FAKE_VOID_FUNC(fake_dealloc, void*, void*)

static void
test__eds_mem__init_NULL_1(void)
{
    /* CUT */
    struct eds_object__mem mem;

    eds_mem__init(&mem, fake_alloc, fake_dealloc, NULL, 1);

    NK_TEST__EXPECT_PTR(NULL);
    NK_TEST__ACTUAL_PTR(eds_mem__context(&mem));
    NK_TEST__EXPECT_SIZE(1);
    NK_TEST__ACTUAL_SIZE(eds_mem__max_size(&mem));
}

static void
test__eds_mem__init_nNULL_M(void)
{
    /* CUT */
    struct eds_object__mem mem;
    uint32_t dummy_context;
#define DUMMY_SIZE 12345

    eds_mem__init(&mem, fake_alloc, fake_dealloc, &dummy_context, DUMMY_SIZE);

    NK_TEST__EXPECT_PTR(&dummy_context);
    NK_TEST__ACTUAL_PTR(eds_mem__context(&mem));
    NK_TEST__EXPECT_SIZE(DUMMY_SIZE);
    NK_TEST__ACTUAL_SIZE(eds_mem__max_size(&mem));
#undef DUMMY_SIZE
}

static void
test__eds_mem__instance_count__zero(void)
{
    struct eds_object__vector fake_mem__instances;
    eds_core__vector_n_entries_fake.return_val = 0;

    /* CUT */
    uint32_t count = eds_mem__instance_count(&fake_mem__instances);

    NK_TEST__EXPECT_UINT(0);
    NK_TEST__ACTUAL_UINT(count);
    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_n_entries_fake.arg0_history[0]);
}

static void
test__eds_mem__instance_count__one(void)
{
    struct eds_object__vector fake_mem__instances;
    eds_core__vector_n_entries_fake.return_val = 1;

    /* CUT */
    uint32_t count = eds_mem__instance_count(&fake_mem__instances);

    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(count);
    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_n_entries_fake.arg0_history[0]);
}

static void
test__eds_mem__find__empty(void)
{
    struct eds_object__vector fake_mem__instances;
    struct eds_object__mem * instance;

    eds_core__vector_n_entries_fake.return_val = 0; /* Tell to MEM that vector is empty */
    /* CUT */
    instance = eds_mem__find(&fake_mem__instances, 1);

    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_n_entries_fake.arg0_history[0]);
    NK_TEST__EXPECT_PTR(NULL);
    NK_TEST__ACTUAL_PTR(instance);
}

static void
test__eds_mem__find__one_entry_no_match(void)
{
    struct eds_object__vector fake_mem__instances;
    struct eds_object__mem fake_mem_instance_in_vector;
    struct eds_object__mem * instance;

    eds_core__vector_n_entries_fake.return_val = 1; /* Tell to MEM that vector has one entry */
    eds_core__vector_peek_fake.return_val = &fake_mem_instance_in_vector;
    fake_mem_instance_in_vector.p__max_size = 2;
    /* CUT */
    instance = eds_mem__find(&fake_mem__instances, 3);

    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_n_entries_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_peek_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(0);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.arg1_history[0]);
    NK_TEST__EXPECT_PTR(NULL);
    NK_TEST__ACTUAL_PTR(instance);
}

static void
test__eds_mem__find__one_entry_exact_match(void)
{
    struct eds_object__vector fake_mem__instances;
    struct eds_object__mem fake_mem_instance_in_vector;
    struct eds_object__mem * instance;

    eds_core__vector_n_entries_fake.return_val = 1; /* Tell to MEM that vector has one entry */
    eds_core__vector_peek_fake.return_val = &fake_mem_instance_in_vector;
    fake_mem_instance_in_vector.p__max_size = 3;
    /* CUT */
    instance = eds_mem__find(&fake_mem__instances, 3);

    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_n_entries_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_peek_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(0);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.arg1_history[0]);
    NK_TEST__EXPECT_PTR(&fake_mem_instance_in_vector);
    NK_TEST__ACTUAL_PTR(instance);
}

static void
test__eds_mem__find__one_entry_over_match(void)
{
    struct eds_object__vector fake_mem__instances;
    struct eds_object__mem fake_mem_instance_in_vector;
    struct eds_object__mem * instance;

    eds_core__vector_n_entries_fake.return_val = 1; /* Tell to MEM that vector has one entry */
    eds_core__vector_peek_fake.return_val = &fake_mem_instance_in_vector;
    fake_mem_instance_in_vector.p__max_size = 4;
    /* CUT */
    instance = eds_mem__find(&fake_mem__instances, 3);

    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_n_entries_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_peek_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(0);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.arg1_history[0]);
    NK_TEST__EXPECT_PTR(&fake_mem_instance_in_vector);
    NK_TEST__ACTUAL_PTR(instance);
}

static void
test__eds_mem__find__two_entries_no_match(void)
{
    struct eds_object__vector fake_mem__instances;
    struct eds_object__mem fake_mem_instances_in_vector[2];
    void * instances_in_vector[] =
        {
        &fake_mem_instances_in_vector[0], &fake_mem_instances_in_vector[1]
        };
    fake_mem_instances_in_vector[0].p__max_size = 1;
    fake_mem_instances_in_vector[1].p__max_size = 2;
    struct eds_object__mem * instance;
    eds_core__vector_n_entries_fake.return_val = 2; /* Tell to MEM that vector has two entries */
    eds_core__vector_peek_fake.return_val_seq = instances_in_vector;
    eds_core__vector_peek_fake.return_val_seq_len = 2;
    /* CUT */
    instance = eds_mem__find(&fake_mem__instances, 3);

    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_n_entries_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_n_entries_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(2);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.call_count);
    NK_TEST__EXPECT_PTR(&fake_mem__instances);
    NK_TEST__ACTUAL_PTR(eds_core__vector_peek_fake.arg0_history[0]);
    NK_TEST__EXPECT_UINT(0);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.arg1_history[0]);
    NK_TEST__EXPECT_UINT(1);
    NK_TEST__ACTUAL_UINT(eds_core__vector_peek_fake.arg1_history[1]);
    NK_TEST__EXPECT_PTR(NULL);
    NK_TEST__ACTUAL_PTR(instance);
}

static void
setup(void)
{
    RESET_FAKE(eds_core__vector_n_entries);
    RESET_FAKE(eds_core__vector_peek);
    RESET_FAKE(eds_core__vector_insert);
    FFF_RESET_HISTORY();
}

void
test_execute(void)
{
    static const struct nk_testsuite__test tests[] =
        {
        NK_TEST__TEST(test__eds_mem__init_NULL_1),
    NK_TEST__TEST(test__eds_mem__init_nNULL_M),
NK_TEST__TEST(test__eds_mem__instance_count__zero),
NK_TEST__TEST(test__eds_mem__instance_count__one),
NK_TEST__TEST(test__eds_mem__find__empty),
NK_TEST__TEST(test__eds_mem__find__one_entry_no_match),
NK_TEST__TEST(test__eds_mem__find__one_entry_exact_match),
NK_TEST__TEST(test__eds_mem__find__one_entry_over_match),
NK_TEST__TEST(test__eds_mem__find__two_entries_no_match),
NK_TEST__TEST_TERMINATE()
}
    ;
    nk_test__run_fixture(tests, setup, NULL, NK_TESTSUITE__FIXTURE_NAME(none));
}
