//
// Created by joshb on 2/12/2025.
//

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "writing.h"
#define TEST(function_name, test_function)\
{\
	printf("Testing %s function...\n", function_name);\
	const bool success = test_function();\
	report_success(function_name, success);\
}\

int TestMakeVariable();
void report_success(const char* function_name, const int success);

int main()
{
	printf("Beginning testing...\n\n");

	TEST("MakeVariable", TestMakeVariable);

	return EXIT_SUCCESS;
}


int TestMakeVariable()
{
	int success = true;

	char* test_cases[] =
		{"a", "a[]", "a][", "a[123]", "a]123[",
		"a[[", "a]]", "a[]]", "a[[]", "a]",
		"a[", "1a[123]", "_a[123]", "A[123]", "a[123] ",
		"a[123a]", "a4", "a4[64]"};
	constexpr int expt_rslts[] =
		{1,	-1,	-1,	123,-1,
		-1,	-1,	-1,	-1,	-1,
		-1,	-1,	123,123,-1,
		123, 1,	64,	};

	static_assert(sizeof test_cases / sizeof test_cases[0] == sizeof expt_rslts / sizeof expt_rslts[0]);
	constexpr size_t number_of_test_cases = sizeof test_cases / sizeof test_cases[0];

	for (size_t i = 0; i < number_of_test_cases; i++)
	{
		if (!strcmp(test_cases[i], "a4[64]"))
			printf("");

		const XilVariable var = xil_MakeVariable(test_cases[i]);
		const int local_success = var.variable_size == expt_rslts[i];
		success = success && local_success;

		if (!local_success)
		{
			printf("\tTest %llu -- creating variable from %s failed. -- expected: %d, got: %d\n", i, test_cases[i], expt_rslts[i], var.variable_size);
			xil_MakeVariable((test_cases[i]));
		}
	}

	return success;
}

void report_success(const char* function_name, const int success)
{
	if (success)
		printf("%s succeeded!\n", function_name);
	else
		printf("%s failed -- error code: %d\n", function_name, success);
}
