/*
 * test_nk_array.cpp
 *
 *  Created on: Feb 21, 2021
 *      Author: nenad
 */

#include "nk_test.h"

#include "fff/fff.h"
#include "sys/eds_mem.h"
#include "eds_object.h"

DEFINE_FFF_GLOBALS

FAKE_VALUE_FUNC(uint32_t, eds_core__vector_n_entries, const struct eds_object__vector *)
FAKE_VALUE_FUNC(void *, eds_core__vector_peek, const struct eds_object__vector *, uint32_t)
FAKE_VALUE_FUNC(bool, eds_core__vector_is_full, const struct eds_object__vector *)
FAKE_VOID_FUNC(eds_core__vector_insert, struct eds_object__vector *, uint32_t, const void *)

FAKE_VALUE_FUNC(void *, fake_alloc, void*, size_t)
FAKE_VOID_FUNC(fake_dealloc, void*, void*)

struct eds_object__vector eds_state__mem_instances;
bool eds_state__has_started;

static void
test__eds_mem__find__empty(void)
{
    struct eds_object__vector fake_mem__instances = { 0 };
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
    struct eds_object__vector fake_mem__instances = { 0 };
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
    struct eds_object__vector fake_mem__instances = { 0 };
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
    struct eds_object__vector fake_mem__instances = { 0 };
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
    struct eds_object__vector fake_mem__instances = { 0 };
    struct eds_object__mem fake_mem_instances_in_vector[2];
    void * instances_in_vector[] =
    {
        &fake_mem_instances_in_vector[0], 
        &fake_mem_instances_in_vector[1]
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
    RESET_FAKE(fake_alloc);
    RESET_FAKE(fake_dealloc);
    FFF_RESET_HISTORY();
}

void
nk_test__execute(void)
{
    static const struct nk_testsuite__test tests[] =
    {
        NK_TEST__TEST(test__eds_mem__find__empty),
        NK_TEST__TEST(test__eds_mem__find__one_entry_no_match),
        NK_TEST__TEST(test__eds_mem__find__one_entry_exact_match),
        NK_TEST__TEST(test__eds_mem__find__one_entry_over_match),
        NK_TEST__TEST(test__eds_mem__find__two_entries_no_match),
        NK_TEST__TEST_TERMINATE()
    };
    nk_test__run_fixture(tests, setup, NULL, NK_TESTSUITE__FIXTURE_NAME(none));
}
