//
// Created by joshb on 2/3/2025.
//

#ifndef WRITING_H
#define WRITING_H

#define XIL_VARIABLE_NAME_LENGTH 32

typedef struct Xil_Variable
{
	int variable_size;
	char variable_name[XIL_VARIABLE_NAME_LENGTH];
} XilVariable;

void write_script(const XilVariable* variables, size_t number_of_variables, int wait_time, const char* assert_expression, bool flip);
XilVariable MakeVariable(const char* declaration);
size_t get_bit_section(size_t value, size_t bit_start, size_t n, bool flip);
char* partial_bit_rep(size_t value, size_t bit_start, size_t n, bool flip, char* dst);


#endif //WRITING_H
