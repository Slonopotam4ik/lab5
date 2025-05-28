#ifndef INPUT_H
#define INPUT_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int int_input(int* value, const char* input_prompt);
int size_t_input(size_t* value, const char* input_prompt);
char *str_input(const char* prompt);
void print_error(int k);


#endif