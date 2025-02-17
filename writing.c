//
// Created by joshb on 2/3/2025.
//

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#ifndef HELPER_H
	#include "helper.h"
#endif
#ifndef WRITING_H
	#include "writing.h"
#endif

/*
 * The number of permutations each bit goes through is equal to:
 * where 'd' is the distance from the left-most bit
 * permutations = 2^(d + 1)
 * Then the total number of writes that needs to be made can be represented by
 * 2^(n + 1) + 2
 *
 * Notes of the writer:
 * Therefore, I propose a 'small' implementation, that doesn't write variables when unnecessary. While at the same time, I somewhat oppose this, as it makes it harder to trace-back values.
 * It is, however, possible to take a line number and replicate the state at that moment.
 * TODO: decoder?
 */


/**
 *
 * @param variable_names An array of variable names.
 * @param number_of_variables Represents the number of bits that change
 * @param wait_time The time in nanoseconds to wait after each instruction in iSim
 * @param assert_expression The expression to be asserted
 * @param flip If true, everything will start true, and will count down
 */
#define tab "    "

void write_script(const XilVariable* variables, const size_t number_of_variables, const int wait_time, const char* assert_expression, const bool flip)
{
	/* TODO: Maybe make the output function optional, between print and write. Will need to learn better C first.
	 * TODO: Implement the use of the variable struct
	 */
	constexpr const char ASSERTION_SCRIPT[]	= "\n" tab "assert %s report \"Error on line %*llu\" severity failure;\n\n";
	constexpr const char FINAL_ASSERTION[]	= "\n" tab "assert false report \"Simulation Finished\" severity failure;\n";
	constexpr const char COMMENT[]			= tab "-- Test %*llu --\n";
	constexpr const char ASSIGNMENT[]		= tab "%s <= '%s';\n";
	const size_t assert_expression_len = (assert_expression ? strlen(assert_expression) : 0);
	int total_bits = 0;
	size_t largest_variable_name = 0;
	size_t largest_string = 0;
	size_t largest_variable_size = 0;
	size_t largest_variable_rep = 0;

	for (size_t i = 0; i < number_of_variables; i++)
	{
		const size_t name_len = strlen(variables[i].variable_name);

		if (name_len + variables[i].variable_size > largest_variable_rep)
			largest_variable_rep = name_len + variables[i].variable_size;
		if (name_len > largest_variable_name)
			largest_variable_name = name_len;
		if (variables[i].variable_size > largest_variable_size)
			largest_variable_size = variables[i].variable_size;
		total_bits += variables[i].variable_size;
	}

	const size_t final_value = power(2, total_bits) - 1;
	const int final_digits = digits(final_value);

	/* This is dumb, but this is calculating the largest string*/
	size_t sizes[] = {
		 (!assert_expression ? 0 : (sizeof(ASSERTION_SCRIPT) - 1) + assert_expression_len + final_digits - (2 + 5)) // - (%s + %*llu
		,(sizeof(FINAL_ASSERTION) - 1)
		,(sizeof(COMMENT) - 1) + final_digits - (5) // - %*llu
		,(sizeof(ASSIGNMENT) - 1) + largest_variable_rep - (2 + 2) // - %s + %d
	};

	constexpr size_t number_of_sizes = sizeof sizes / sizeof sizes[0];
	for (size_t i = 0; i < number_of_sizes; i++)
		if (sizes[i] > largest_string)
			largest_string = sizes[i];


	/* The actual writing part */
	const size_t buf_size = largest_string * sizeof(char) + 1;
	char* buf = malloc(buf_size);
	char* num_rep_buf = malloc(largest_variable_size + 1);

	for(size_t i = 0; i <= final_value; ++i)
	{
		sprintf(buf, COMMENT, final_digits, i);
		printf(center_align_in_place(buf, buf_size - 1));

		for(size_t j = 0, bits_expressed = 0; j < number_of_variables; ++j)
		{
			//Left to right - convert to bool (get cursor bit, select bit) flip -- if flip = 0
			const int bit = (!!((1ull << j) & i)) ^ flip;
			sprintf(buf, ASSIGNMENT, variables[j].variable_name, itoa(bit, num_rep_buf, 2)); //FIXME: This is only good for single bits right now
			if (variables[j].variable_size > 1)
				strreplace(buf, '\'', '"');
			printf(center_align_in_place(buf, buf_size - 1));
		}
		if (assert_expression)
		{
			sprintf(buf, ASSERTION_SCRIPT, assert_expression, final_digits, i);
			center_align_in_place(buf, buf_size);
			printf(buf);
		}
		else
			printf("\n");
	}

	printf(FINAL_ASSERTION);
	free(buf);
	free(num_rep_buf);
}

//
size_t get_bit_section(const size_t value, const size_t bit_start, const size_t n, const bool flip)
{
	assert(n + bit_start != sizeof(size_t) * 8);
	// Create a block of n 1's, then move them over to the start
	const size_t flag = (pow(2, n) - 1) * pow(2, bit_start);

	// See which 1's match in the selected region, flip it if applicable, then return to lowest value containing that bit-sequence
	return ((value & flag) ^ (flip ? flag : 0 )) / pow(2, bit_start);
}

char* partial_bit_rep(const size_t value, const size_t bit_start, const size_t n, const bool flip, char* dst)
{
	const size_t sequence = get_bit_section(value, bit_start, n, flip);
	for (size_t i = 0; i < n; i++)
	{
		const int bit = !!((1ull << i) & sequence);
		dst[i] = bit ? '1' : '0';
	}
	dst[n] = '\0';
	return dst;
};

XilVariable MakeVariable(const char* declaration)
{
	XilVariable var = {
		.variable_size = -1,
		.variable_name = {0},
	};

	const char* bracket_opening = strchr(declaration, '[');
	const char* bracket_closing = strchr(declaration, ']');
	const char* first_bracket = (bracket_opening < bracket_closing ? bracket_opening : bracket_closing );
	const int name_length = first_bracket ? (int)(first_bracket - declaration) : (int)strlen(declaration); // First_bracket can be 0. Therefore this must be signed.

	// Return invalid variable if it starts with an illegal character.
	if (!(isalpha(declaration[0]) || declaration[0] == '_' || !declaration[0]))
		return var;

	// Invalid if it contains any illegal characters
	for (size_t i = 0; &(declaration[i]) < bracket_opening; i++)
		if (!(isalnum(declaration[i]) || declaration[i] == '_' || !declaration[i]))
			return var;

	// A closing bracket must exist after wherever an opening one does, but with information between the two
	// there may only be one of each bracket, which must exist in a pair, if at all. The closing bracket must
	// be the last character of the declaration.
	if ((bracket_opening || bracket_closing) && (bracket_closing - bracket_opening) <= 0)
		return var;
	if (bracket_closing && bracket_closing[1] != '\0')
		return var;

	//check for too many brackets
	if (bracket_opening && bracket_closing)				//this is unnecessary, but completes the logic
		if (strchr(bracket_opening + 1, '['))//	|| strchr(bracket_closing + 1, ']'))
			return var;
	if (bracket_closing && !bracket_opening)
		return var;

	//Set the variable name.
	var.variable_name[name_length] = '\0';
	memcpy(var.variable_name, declaration, name_length);

	if (!bracket_opening)
	{
		var.variable_size = 1;
		return var;
	}

	const int var_size = atoi(bracket_opening + 1); // NOLINT(*-err34-c)
	var.variable_size = (var_size ? var_size : -1);

	return var;
}

#undef tab