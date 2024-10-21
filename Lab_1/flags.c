#include "main.h"
#include <stdio.h>

enum Errors find_flag_in_input(char **argv, int *rez_flag, int *rez_nam){
    int is_find_flag = 0, is_find_namber = 0;
    for(int i=1;i<3;i++){
        if ((argv[i][0] == '-' || argv[i][0] == '/') && argv[i][2] == '\0' && !(argv[i][1] - '0' >= 0 && argv[i][1] - '0' <= 9)){
            is_find_flag = 1;
            *rez_flag = i;
        }

        if (((argv[i][0] - '0' >= 0 && argv[i][0] - '0' <= 9) || argv[i][0] == '-') && is_find_namber == 0){
            is_find_namber = 1;
            for(int o=1;o<(int)sizeof(argv[i]);o++){
                if (argv[i][o] == '\0'){
                    if (!(argv[i][o-1] - '0' >= 0 && argv[i][o-1] - '0' <= 9))
                        is_find_namber = 0;
                    break;
                }
                if (!(argv[i][o] - '0' >= 0 && argv[i][o] - '0' <= 9))
                    is_find_namber = 0;
            }
            
            if (is_find_namber == 1)
                *rez_nam = i;
        }
    }
    if (is_find_namber == 0 || is_find_flag == 0)
        return INVALID_INPUT;
    return OK;
}

enum Errors convert_str_to_int(const char *str, long long int * result, int base)
{
    char *endptr;
    *result = strtoll(str, &endptr, base);

    if (*result == LLONG_MAX || *result == LLONG_MIN)
        return INVALID_INPUT;
    else if (*endptr != '\0') 
        return INVALID_INPUT;

    return OK;
}

enum Errors find_multiples_nam(long long int nam, int** rez, int* col_vo){
    if (nam == 0) return INVALID_INPUT;
    *col_vo = (int)(100 / llabs(nam));

    *rez = (int*)malloc(*col_vo*sizeof(int));
    if (*rez == NULL){
        //free(*rez);
        return INVALID_MEMORY;
    } 

    for(int i=0;i<*col_vo;i++)
        (*rez)[i] = (int)(i+1)*llabs(nam);
    
    return OK;
}

/*
-1 - составное число
0 - не простое и не составное
1 - простое число
*/
enum Errors is_prime_number(long long int nam, int* rez){
    if (nam <= 1){
        *rez = 0;
        return OK;
    }
    
    for (long long int i = 2; i*i <= nam; i++) {
        if(nam % i == 0){
            *rez = -1;
            return OK;
        }
    }
    
    *rez = 1;
    return OK;
}

enum Errors convert_int_to_16x_str (long long int nam, char** rez, int* col_vo){
    char* mas_simb = "0123456789ABCDEF";
    int BAZE = 16;
    *col_vo = 0;
    long long int nam_copy = nam;

    while (nam_copy != 0)
    {
        nam_copy /= BAZE;
        (*col_vo)++;
    }

    if (nam == 0) *col_vo = 1;


    *rez = (char*)malloc(*col_vo*sizeof(char));
    if (rez == NULL){
        free(*rez);
        return INVALID_MEMORY;
    } 

    nam_copy = nam;
    for (long long int i = 0; i < *col_vo; i++)
    {
        (*rez)[*col_vo - i - 1] = mas_simb[llabs(nam_copy%BAZE)];
        nam_copy/=BAZE;
    }

    if (nam < 0)
    {
        (*col_vo)++;
        char *for_realloc = realloc(*rez, (*col_vo) * sizeof(char));
        if (for_realloc == NULL) {
            free(for_realloc);
            return INVALID_MEMORY;
        }

        *rez = for_realloc;
        (*rez)[*col_vo - 1] = '-';
    }
    

    return OK;
}

enum Errors get_table_of_degrees(long long int nam, long long int*** rez){
    if (nam > 10 || nam < 1)
        return INVALID_INPUT;
    
    *rez = (long long int **) malloc(nam * sizeof(long long int*));
    if (*rez == NULL){
        free(*rez);
        return INVALID_MEMORY;
    } 
    for (int i = 0; i < nam; i++)
    {
        (*rez)[i] = (long long int *)malloc(10 * sizeof(long long int));
        if ((*rez)[i] == NULL){
            for (int o = 0; o <= i; o++)
                free((*rez)[i]);
            free(*rez);
            
            return INVALID_MEMORY;
        } 
    }
    
    for (int i = 1; i <= nam; i++){
        for (int o = 1; o <= 10; o++){
            if (i == 1)
                (*rez)[i-1][o-1] = (long long int)o;
            else
                (*rez)[i-1][o-1] = (long long int)(*rez)[i-2][o-1] * (long long int)o;
        }
    }
    return OK;
}

enum Errors summ_natur_nambers(long long int nam,long long int* rez){
    if (nam <= 1)
        *rez = 1;
    else if (nam % 2 == 0){
        if (LLONG_MAX / (nam/2) / (nam) == 0)
            return INVALID_MEMORY;
        else
            *rez = nam/2 * (nam + 1);
    }
    else{ 
        if (LLONG_MAX / (nam/2) / (nam) == 0)
            return INVALID_MEMORY;
        *rez = nam/2 * (nam + 1) + (nam + 1) / 2;
    }
    return OK;
}

enum Errors my_factorial(long long int nam, long long int* rez){
    if (nam < 0){
        *rez = 0;
        return OK;
    }
    *rez = 1;
    for (long long int i = 2; i <= nam; i++){
        if (LLONG_MAX / i / (*rez) == 0)
            return INVALID_MEMORY;
        (*rez) *= i;
    }
        
    return OK;
}

