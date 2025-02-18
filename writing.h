//
// Created by joshb on 2/3/2025.
//

#ifndef WRITING_H
#define WRITING_H

typedef enum {
	XIL_VARIABLE_NAME_LENGTH = 32,
	XIL_NUMBER_OF_SECTIONS = 4,
} XIL_CONSTANTS;

typedef struct Xil_Variable
{
	int variable_size;
	char variable_name[XIL_VARIABLE_NAME_LENGTH];
} XilVariable;

void xil_WriteScript(const XilVariable* variables, size_t number_of_variables, int wait_time, const char* assert_expression, bool flip);
XilVariable xil_MakeVariable(const char* declaration);
size_t xil_GetBitSection(size_t value, size_t bit_start, size_t n, bool flip);
char* xil_PartialBitRep(size_t value, size_t bit_start, size_t n, bool flip, char* dst);


#endif //WRITING_H
