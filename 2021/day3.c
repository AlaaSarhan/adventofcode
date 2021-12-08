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

void part1() {
    intptr_t* results = (intptr_t*) malloc(2000 * sizeof(intptr_t));
    int total_words = map_lines("day3-input.txt", results, 2000, &add_to_bit_position_sums);
    free(results);
    unsigned int gamma, epsilon;
    calc_gamma_and_epsilon(&gamma, &epsilon, word_size, total_words);
    printf("Gamma: %d, Epsilon: %d ==> ", gamma, epsilon);
    printf("Part 1: %d\n", gamma * epsilon);
}

int convert_bits_to_int(char* line) {
    int val = 0;
    for (int i = 0; i < strlen(line); i++) {
        val <<= 1;
        val += line[i] == '1' ? 1 : 0;
    }
    return val;
}

intptr_t map_line_to_int(char* line) {
    char* new_line = strchr(line, '\n');
    if (new_line) {
        *new_line = '\0';
    }
    return (intptr_t) line;
}

int sum_bits_in_position(int position, char** numbers, int numbers_count) {
    int sum = 0;
    for (int i = 0; i < numbers_count; i++) {
        sum += numbers[i][position] == '1' ? 1 : 0;
    }
    return sum;
}

int find_matching_number(int position, char default_match, char comparison, char** numbers, int numbers_count) {
    if (numbers_count == 0) {
        printf("Something is wrong. Can't find a match %d %c\n", default_match, comparison);
        exit(2021);
    }
    if (numbers_count == 1) {
        return convert_bits_to_int(numbers[0]);
    }

    int new_numbers_count = 0;
    char** new_numbers = (char**) malloc(sizeof(char**) * numbers_count);

    int sum = sum_bits_in_position(position, numbers, numbers_count);
    int half_count = numbers_count / 2;
    int is_odd = half_count * 2 != numbers_count;
    char most_common = sum > half_count ? '1' : sum < ( is_odd ? half_count + 1 : half_count) ? '0' : '-';

    for (int i = 0; i < numbers_count; i++) {
        char bit = numbers[i][position];
        if( // include number
            (most_common == '-' && bit == default_match) ||
            (most_common != '-' && (
                (comparison == '>' && bit == most_common) ||
                (comparison == '<' && bit != most_common)
            ))
        ) {
            new_numbers[new_numbers_count++] = numbers[i];
        }
    }

    return find_matching_number(position + 1, default_match, comparison, new_numbers, new_numbers_count);
}

void part2() {
    char** numbers = (char**) malloc(2000 * sizeof(intptr_t));
    int numbers_count = map_lines("day3-input.txt", (intptr_t*) numbers, 2000, &map_line_to_int);
    int oxygen_gen_rating = find_matching_number(0, '1', '>', numbers, numbers_count);
    int co2_scrub_rating = find_matching_number(0, '0', '<', numbers, numbers_count);
    printf("O2 GenRate: %d, CO2 ScrubRate: %d ==> Part 2: %d\n", oxygen_gen_rating, co2_scrub_rating, oxygen_gen_rating * co2_scrub_rating);
}

int main() {
    part1();
    part2();
}