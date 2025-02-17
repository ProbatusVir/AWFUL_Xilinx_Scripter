#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>

#include "helper.h"
#include "writing.h"

#define CONSOLE_SPEAKER_COLOR	"\33[33m"
#define USER_SPEAKER_COLOR		"\33[37m"
#define ERROR_COLOR				"\33[31m"

#define VARIABLE_NAME_PROMPT	CONSOLE_SPEAKER_COLOR "Name of variable %llu: " USER_SPEAKER_COLOR
#define VARIABLE_COUNT_PROMPT	CONSOLE_SPEAKER_COLOR "Number of variables: " USER_SPEAKER_COLOR
#define ASSERTION_PROMPT		CONSOLE_SPEAKER_COLOR "Enter assert expression (eg. O = Xin xor Yin) or press enter.\nExpression: " USER_SPEAKER_COLOR

int main(void)
{
	constexpr size_t MAX_BITS = sizeof(size_t) * 8;
	constexpr size_t VARIABLE_VALUE_DIGITS	= digits(MAX_BITS);
	constexpr size_t assert_buffer_size		= 256;
	constexpr size_t name_prompt_size = VARIABLE_VALUE_DIGITS + (sizeof(VARIABLE_COUNT_PROMPT) - 3);
	/* TODO: implement write to file option
	 * TODO: Add a way to introduce constants.
	 */

	/* Get Inputs */

	//Get a number of variables between 1 and 64
	const size_t number_of_variables = cget_pos_int(1, MAX_BITS, VARIABLE_COUNT_PROMPT);
	XilVariable* variables = malloc(number_of_variables * sizeof(XilVariable)); //at most this will be 2304 bytes
	size_t total_bits = 0;
	//Get all the names
	char name_prompt_buffer[name_prompt_size] = {0};
	char* name_input_buffer[XIL_VARIABLE_NAME_LENGTH + 1] = {0};
	for(size_t variable_index = 0; variable_index < number_of_variables; ++variable_index)
	{
		variables[variable_index] = (XilVariable){-1};
		sprintf(name_prompt_buffer, VARIABLE_NAME_PROMPT, variable_index + 1ull);

		while (variables[variable_index].variable_size < 1 || variables[variable_index].variable_size > MAX_BITS)
		{
			cget_line(name_input_buffer, XIL_VARIABLE_NAME_LENGTH, name_prompt_buffer);
			variables[variable_index] = MakeVariable(name_input_buffer);
		}
		total_bits += variables[variable_index].variable_size;
	}

	if (total_bits > MAX_BITS)
	{
		printf(ERROR_COLOR "ERROR: You have specified too many inputs. Please keep in the range of 1-64.\n" USER_SPEAKER_COLOR);
		system("PAUSE");
		return EXIT_FAILURE;
	}


	//Get assertion (if any) TODO: Uh-oh, this is kinda big.
	char* assertion = malloc(assert_buffer_size);
	memset(assertion, '\0', assert_buffer_size);
	cget_line(assertion, assert_buffer_size - 1, ASSERTION_PROMPT);

	if (assertion[0] == '\0')
	{
		free(assertion);
		assertion = nullptr;
	}

	write_script(variables, number_of_variables, 10, assertion, 0);
	//Cleanup
	free(variables);

	printf("\nThis program has finished executing.\n");
	system("PAUSE");
	return 0;
}

