#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

char* itob(uintmax_t i, bool trim_leading_zeros) {
    uintmax_t max_b_size = sizeof(uintmax_t) * 8;
    char* b = (char*) malloc(max_b_size + 1);
    b[max_b_size] = '\0';

    char* current_bit = b + max_b_size - 1;
    for (; current_bit > b; current_bit--) {
        if (i == 0) {
            if (trim_leading_zeros && current_bit < (b + max_b_size - 1)) {
                break;
            }
            *current_bit = '0';
        } else {
            *current_bit = i & 1 ? '1' : '0';
            i >>= 1;
        }
    }

    return current_bit + 1;
}