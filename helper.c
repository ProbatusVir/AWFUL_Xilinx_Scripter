//
// Created by joshb on 2/3/2025.
//

#include "helper.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>


char* strreplace(char* src, const char to_replace, const char replacement)
{
	const size_t src_len = strlen(src);
	for (size_t i = 0; i < src_len; i++)
		if (src[i] == to_replace)
			src[i] = replacement;
	return src;
}

//copies n bytes from src to dest, inserting spaces until it is aligned to the center.
const char* center_align_in_place(char* src, const size_t n)
{
	const size_t src_len = strlen(src);
	assert(n > src_len);

	char* cp = malloc(n);
	memcpy(cp, src, n);
	const size_t margin = (n - src_len) / 2;

	memset(src, ' ', margin);
	memcpy(src + margin, cp, src_len);
	src[margin + src_len] = '\0';

	free(cp);
	return src;
}

/**
 * Get a positive integer from the console. If min and max are 0, any positive integer is acceptable.
 * @param prompt String prompt. nullptr or "" are acceptable. If a newline or colon is desired, it must be provided.
 * @param min The minimum value a user can provide.
 * @param max The maximum value a user can provide.
 * @return A user provided positive integer.
 */
size_t cget_pos_int(const size_t min, const size_t max, const char* prompt)
{
	if (!prompt)
		prompt = "";

	printf(prompt);

	size_t response = 0;
	for (int error = 0; !error || response > max || response < min;) {
		error = scanf("%d", &response);
		getchar();
	}

	return response;
}

/*
 * Reads n bytes into the buffer.
 */
char* cget_line(char* buffer, const int n, const char* prompt)
{
	assert(n > 0 && buffer);
	if (!prompt)
		prompt = "";

	printf(prompt);

	char* string = fgets(buffer, n, stdin);
	const size_t len = strlen(string);
	if (string[len - 1] == '\n')
		string[len - 1] = '\0';
	return string;
}

size_t num_length(const int n)
{
	return digits(n) + signbit(n);
}

size_t snum_length(const size_t n)
{
	return digits(n);
}

size_t power(const int a, const int b)
{
	if (!b)
		return 1;

	size_t sum = a;

	for (size_t i = 0; i < b - 1; i++)
		sum *= a;

	return sum;
}