//
// Created by joshb on 2/3/2025.
//

#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>

#define digits(n) (1 + floor(log10(n)))

size_t cget_pos_int(size_t min,size_t max, const char* prompt);
char* cget_line(char* buffer,int n, const char* prompt);
size_t num_length(int n); //I don't know if doing this with doubles is smart.
size_t snum_length(size_t n);
size_t power(int a, int b);
const char* center_align_in_place(char* src, size_t n);
/**
 * Replace characters in a null-terminated string
 * @param src The string to be affected.
 * @param to_replace The character you want to be replaced.
 * @param replacement The character to replace the character with
 * @return The altered string.
 */
char* strreplace(char* src, char to_replace, char replacement);



#endif //HELPER_H
