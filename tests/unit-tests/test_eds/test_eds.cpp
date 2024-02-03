/*
 * test_nk_array.cpp
 *
 *  Created on: Feb 21, 2021
 *      Author: nenad
 */

#include <gtest/gtest.h>

#include "eds.h"

TEST(ErrorToString, ErrorNone)
{
	const char * error_string;

	error_string = eds__error_to_str(EDS__ERROR_NONE);
	EXPECT_STREQ(error_string, "none");
}
