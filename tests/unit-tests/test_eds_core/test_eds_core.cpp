/*
 * test_nk_array.cpp
 *
 *  Created on: Feb 21, 2021
 *      Author: nenad
 */

#include <stdint.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "sys/eds_core.h"
#include "eds_object.h"

TEST(vector, initialization_n_size)
{
#define DUMMY_VECTOR_SIZE 10
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, initialization_n_entries_0)
{
#define DUMMY_VECTOR_SIZE 10
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 0);
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, initialization_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_VECTOR_SIZE
}

/* Insert 1 */

TEST(vector, insert_1_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_1_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_1_n_entries_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 1);
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_1_peek_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);

	EXPECT_EQ(*(uint32_t* )eds_core__vector_peek(&vector, 0), DUMMY_INSERT_VALUE);
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 1, remove 1 */

TEST(vector, insert_1_remove_1_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_1_remove_1_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_remove(&vector, 0);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_1_remove_1_n_entries_0)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_VALUE 3
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 0);
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2 */

TEST(vector, insert_2_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_n_entries_2)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 2);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_peek_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);

	EXPECT_EQ(*(uint32_t* )eds_core__vector_peek(&vector, 0), DUMMY_INSERT_1_VALUE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_peek_2)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);

	EXPECT_EQ(*(uint32_t* )eds_core__vector_peek(&vector, 1), DUMMY_INSERT_2_VALUE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2 (reversed) */

TEST(vector, insert_2r_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 0, &dummy_insert_2);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2r_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 0, &dummy_insert_2);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2r_n_entries_2)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 0, &dummy_insert_2);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 2);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2r_peek_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 0, &dummy_insert_2);

	EXPECT_EQ(*(uint32_t* )eds_core__vector_peek(&vector, 0), DUMMY_INSERT_2_VALUE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2r_peek_2)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 0, &dummy_insert_2);

	EXPECT_EQ(*(uint32_t* )eds_core__vector_peek(&vector, 1), DUMMY_INSERT_1_VALUE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2, Remove 1f */

TEST(vector, insert_2_remove_1f_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_1f_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 0);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_1f_n_entries_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 1);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_1f_peek_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(*(uint32_t* )eds_core__vector_peek(&vector, 0), DUMMY_INSERT_2_VALUE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2, Remove 1b */

TEST(vector, insert_2_remove_1b_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 1);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_1b_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 1);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_1b_n_entries_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 1);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 1);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_1b_peek_1)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 1);

	EXPECT_EQ(*(uint32_t* )eds_core__vector_peek(&vector, 0), DUMMY_INSERT_1_VALUE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2, Remove 2 */

TEST(vector, insert_2_remove_2_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 1);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_2_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 1);
	eds_core__vector_remove(&vector, 0);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_2_n_entries_0)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 1);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 0);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2, Remove 2 (reversed)*/

TEST(vector, insert_2_remove_2r_n_size)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 0);
	eds_core__vector_remove(&vector, 1);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_2r_is_full_false)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 0);
	eds_core__vector_remove(&vector, 1);

	EXPECT_FALSE(eds_core__vector_is_full(&vector));
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_2_remove_2r_n_entries_0)
{
#define DUMMY_VECTOR_SIZE 10
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_remove(&vector, 0);
	eds_core__vector_remove(&vector, 1);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), 0);
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_3_n_size_3)
{
#define DUMMY_VECTOR_SIZE 3
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
#define DUMMY_INSERT_3_VALUE 7
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
	const uint32_t dummy_insert_3 = DUMMY_INSERT_3_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_insert(&vector, 2, &dummy_insert_3);

	EXPECT_EQ(eds_core__vector_n_size(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_3_VALUE
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

TEST(vector, insert_3_n_entries_3)
{
#define DUMMY_VECTOR_SIZE 3
#define DUMMY_INSERT_1_VALUE 3
#define DUMMY_INSERT_2_VALUE 5
#define DUMMY_INSERT_3_VALUE 7
	struct eds_object__vector vector;
	uint32_t dummy_entries[DUMMY_VECTOR_SIZE];
	const uint32_t dummy_insert_1 = DUMMY_INSERT_1_VALUE;
	const uint32_t dummy_insert_2 = DUMMY_INSERT_2_VALUE;
	const uint32_t dummy_insert_3 = DUMMY_INSERT_3_VALUE;

	/* CUT */
	eds_core__vector_init(&vector, dummy_entries, sizeof(dummy_entries[0]), DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert_1);
	eds_core__vector_insert(&vector, 1, &dummy_insert_2);
	eds_core__vector_insert(&vector, 2, &dummy_insert_3);

	EXPECT_EQ(eds_core__vector_n_entries(&vector), DUMMY_VECTOR_SIZE);
#undef DUMMY_INSERT_3_VALUE
#undef DUMMY_INSERT_2_VALUE
#undef DUMMY_INSERT_1_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 1 (leak) */

TEST(vector, insert_1_leak)
{
#define DUMMY_VECTOR_SIZE 1
#define DUMMY_INSERT_VALUE 3
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
		.begin_guard = BEGIN_GUARD_VALUE,
		.end_guard = END_GUARD_VALUE
	};
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(
			&vector,
			&dummy_entries.entries[0],
	        sizeof(dummy_entries.entries[0]),
			DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);

	EXPECT_EQ(dummy_entries.begin_guard, BEGIN_GUARD_VALUE);
	EXPECT_EQ(dummy_entries.end_guard, END_GUARD_VALUE);
#undef END_GUARD_VALUE
#undef BEGIN_GUARD_VALUE
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 1, Remove 1 (leak) */

TEST(vector, insert_1_remove_1_leak)
{
#define DUMMY_VECTOR_SIZE 1
#define DUMMY_INSERT_VALUE 3
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
		.begin_guard = BEGIN_GUARD_VALUE,
		.end_guard = END_GUARD_VALUE
	};
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(
			&vector,
			&dummy_entries.entries[0],
	        sizeof(dummy_entries.entries[0]),
			DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(dummy_entries.begin_guard, BEGIN_GUARD_VALUE);
	EXPECT_EQ(dummy_entries.end_guard, END_GUARD_VALUE);
#undef END_GUARD_VALUE
#undef BEGIN_GUARD_VALUE
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2 (leak) */

TEST(vector, insert_2_leak)
{
#define DUMMY_VECTOR_SIZE 2
#define DUMMY_INSERT_VALUE 3
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
		.begin_guard = BEGIN_GUARD_VALUE,
		.end_guard = END_GUARD_VALUE
	};
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(
			&vector,
			&dummy_entries.entries[0],
	        sizeof(dummy_entries.entries[0]),
			DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_insert(&vector, 0, &dummy_insert);

	EXPECT_EQ(dummy_entries.begin_guard, BEGIN_GUARD_VALUE);
	EXPECT_EQ(dummy_entries.end_guard, END_GUARD_VALUE);
#undef END_GUARD_VALUE
#undef BEGIN_GUARD_VALUE
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2, Remove 1 (leak) */

TEST(vector, insert_2_remove_1_leak)
{
#define DUMMY_VECTOR_SIZE 2
#define DUMMY_INSERT_VALUE 3
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
		.begin_guard = BEGIN_GUARD_VALUE,
		.end_guard = END_GUARD_VALUE
	};
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(
			&vector,
			&dummy_entries.entries[0],
	        sizeof(dummy_entries.entries[0]),
			DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(dummy_entries.begin_guard, BEGIN_GUARD_VALUE);
	EXPECT_EQ(dummy_entries.end_guard, END_GUARD_VALUE);
#undef END_GUARD_VALUE
#undef BEGIN_GUARD_VALUE
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

/* Insert 2, Remove 2 (leak) */

TEST(vector, insert_2_remove_2_leak)
{
#define DUMMY_VECTOR_SIZE 2
#define DUMMY_INSERT_VALUE 3
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
		.begin_guard = BEGIN_GUARD_VALUE,
		.end_guard = END_GUARD_VALUE
	};
	const uint32_t dummy_insert = DUMMY_INSERT_VALUE;

	/* CUT */
	eds_core__vector_init(
			&vector,
			&dummy_entries.entries[0],
	        sizeof(dummy_entries.entries[0]),
			DUMMY_VECTOR_SIZE);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_insert(&vector, 0, &dummy_insert);
	eds_core__vector_remove(&vector, 0);
	eds_core__vector_remove(&vector, 0);

	EXPECT_EQ(dummy_entries.begin_guard, BEGIN_GUARD_VALUE);
	EXPECT_EQ(dummy_entries.end_guard, END_GUARD_VALUE);
#undef END_GUARD_VALUE
#undef BEGIN_GUARD_VALUE
#undef DUMMY_INSERT_VALUE
#undef DUMMY_VECTOR_SIZE
}

