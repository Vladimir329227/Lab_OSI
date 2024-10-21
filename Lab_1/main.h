#ifndef MAIN_H_FLAG
#define MAIN_H_FLAG

#include <errno.h>
#include <stdlib.h>



enum Errors
{
    OK,
    INVALID_MEMORY,
    INVALID_INPUT,
};

enum Errors find_flag_in_input(char **argv, int *rez_flag, int *rez_nam);

enum Errors convert_str_to_int (const char *str, long long int * result, int base);

enum Errors find_multiples_nam(long long int nam, int** rez, int* col_vo);

enum Errors is_prime_number(long long int nam, int* rez);

enum Errors convert_int_to_16x_str (long long int nam, char** rez, int* col_vo);

enum Errors get_table_of_degrees(long long int nam, long long int*** rez);

enum Errors summ_natur_nambers(long long int nam, long long int* rez);

enum Errors my_factorial(long long int nam, long long int* rez);

#endif