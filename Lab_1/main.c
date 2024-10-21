#include <stdio.h>
#include "main.h"

int main(int argc, char **argv) { 
    if (argc >= 3){
        int cur_place_flag, cur_place_nam;
        switch (find_flag_in_input(argv, &cur_place_flag, &cur_place_nam))
        {
        case INVALID_INPUT:
            printf("INVALID_INPUT");
            return INVALID_INPUT;
        case INVALID_MEMORY:
            printf("INVALID_MEMORY");
            return INVALID_MEMORY;
        default:
            break;
        } 

        long long int namber;
        switch (convert_str_to_int(argv[cur_place_nam], &namber, 10))
        {
        case INVALID_INPUT:
            printf("INVALID_INPUT in namber");
            return INVALID_INPUT;
        case INVALID_MEMORY:
            printf("INVALID_MEMORY in namber");
            return INVALID_MEMORY;
        default:
            break;
        } 

        enum Errors err = OK;

        switch (argv[cur_place_flag][1])
        {
        case 'h':
            int* result_h = NULL, col_vo_h;
            err = find_multiples_nam(namber, &result_h, &col_vo_h);
            if (err != OK) break;
            if (col_vo_h == 0)
                printf("Nothing found");
            for (int i = 0; i < col_vo_h; i++)
                printf("%i ",result_h[i]);
            printf("\n");
            free(result_h);
            break;
        case 'p':
            int rez_p;
            err = is_prime_number(namber, &rez_p);
            if (err != OK) break;
            if (rez_p == 0)
                printf("%lli - not a prime and not a composite number", namber);
            else if (rez_p == -1)
                printf("%lli - composite number", namber);
            else if (rez_p == 1)
                printf("%lli - prime number", namber);
            printf("\n");
            break;
        case 's':
            char* rez_s;
            int col_vo_s;
            err = convert_int_to_16x_str(namber, &rez_s, &col_vo_s);
            if (err != OK) break;
            for (int i = 0; i < col_vo_s; i++)
                printf("%c ", rez_s[i]);
            printf("\n");
            free(rez_s);
            break;
        case 'e':
            long long int** rez_e;
            err = get_table_of_degrees(namber, &rez_e);
            if (err != OK) break;
            for (int i = 0; i <= namber; i++){                
                for (int o = 0; o < 10; o++)
                    printf("%lli ", rez_e[i][o]);
                printf("\n");
                if (namber == 1) break;
            }
            for (int i = 0; i <= namber; i++)
                free(rez_e[i]);
            free(rez_e);
            break;
        case 'a':
            long long int rez_a;
            err = summ_natur_nambers(namber, &rez_a);
            if (err != OK) break;
            printf("%lli - sum of all numbers from 1 to %lli\n", rez_a, namber);
            break;
        case 'f':
            long long int rez_f;
            err = my_factorial(namber, &rez_f);
            if (err != OK) break;
            printf("%lli = !%lli\n", rez_f, namber);
            break;
        default:
            printf("Flag not found\n");
            break;
        }

        switch (err)
        {
        case INVALID_INPUT:
            printf("INVALID_INPUT in %c", argv[cur_place_flag][1]);
            return INVALID_INPUT;
        case INVALID_MEMORY:
            printf("INVALID_MEMORY in %c", argv[cur_place_flag][1]);
            return INVALID_MEMORY;
        default:
            break;
        } 
    }
    else {
        printf("INVALID_INPUT");
        return INVALID_INPUT;
    }
    
    return OK;
}
