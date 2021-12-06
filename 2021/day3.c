#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "includes/files.c"

int* bit_position_sums = NULL;
int word_size = 0;
intptr_t add_to_bit_position_sums(char* line) {
    if (bit_position_sums == NULL) {
        char* bits = (char*) malloc(sizeof(char) * 100);
        sscanf(line, "%s", bits);
        word_size = strlen(bits);
        free(bits);
        bit_position_sums = (int*) calloc(word_size, sizeof(int));
    }

    for (int i = 0; i < strlen(line); i++) {
        bit_position_sums[i] += line[i] == '1' ? 1 : 0;
    }

    // we only need the sums. mapped values don't matter
    return (intptr_t) NULL;
}

void calc_gamma_and_epsilon(
    unsigned int* gamma_out,
    unsigned int* epsilon_out,
    int word_size,
    int total_words
) {
    int half_total_words = total_words / 2;
    unsigned int gamma = 0;
    unsigned int epsilon = 0;

    for (int i = 0; i < word_size; i++) {
        gamma <<= 1;
        epsilon <<= 1;
        if (bit_position_sums[i] >= half_total_words) {
            gamma += 1;
        } else {
            epsilon += 1;
        }
    }

    *gamma_out = gamma;
    *epsilon_out = epsilon;
}


void part1(int total_words, int word_size, int* bit_position_sums) {
    unsigned int gamma, epsilon;
    calc_gamma_and_epsilon(&gamma, &epsilon, word_size, total_words);
    printf("Gamma: %d, Epsilon: %d ==> ", gamma, epsilon);
    printf("Part 1: %d\n", gamma * epsilon);
}

void part2() {
    // TODO
}

int main() {
    intptr_t* results = (intptr_t*) malloc(2000 * sizeof(intptr_t));
    int total_words = map_lines("day3-input.txt", results, 2000, &add_to_bit_position_sums);
    free(results);
    part1(total_words, word_size, bit_position_sums);
}