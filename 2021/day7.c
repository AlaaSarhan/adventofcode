#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "includes/files.c"
#include "includes/memory.c"

static int MEMORY_BUCKET_DEFAULT_SIZE = 100;

typedef struct submarine {
    int level;
} SUBMARINE;

intptr_t calc_part_1_cost(intptr_t submarine, intptr_t current_level, intptr_t total_cost) {
    return total_cost + abs(((SUBMARINE*) submarine)->level - *((int*) current_level));
}

intptr_t calc_part_2_cost(intptr_t submarine, intptr_t current_level, intptr_t total_cost) {
    int steps = abs(((SUBMARINE*) submarine)->level - *((int*) current_level));
    return total_cost + (steps * steps + steps) / 2;
}

MEMORY_BUCKET* constrcut_levels(char* input) {
    char* crab_level = strtok(input, ",");

    SUBMARINE* element = (SUBMARINE*) malloc(sizeof(SUBMARINE));
    element->level = atoi(crab_level);
    MEMORY_BUCKET* submarines = mb_add_element(NULL, (intptr_t) element, MEMORY_BUCKET_DEFAULT_SIZE);

    while (crab_level = strtok(NULL, ",")) {
        SUBMARINE* element = (SUBMARINE*) malloc(sizeof(SUBMARINE));
        element->level = atoi(crab_level);
        mb_add_element(submarines, (intptr_t) element, MEMORY_BUCKET_DEFAULT_SIZE);
    }

    return submarines;
}

void main() {
    char* input = read_first_line("day7-input.txt");
    MEMORY_BUCKET* submarines = constrcut_levels(input);

    uintmax_t subs_count;
    SUBMARINE** subs_arr = (SUBMARINE**) mb_to_array(submarines, &subs_count);

    int min_level = INT_MAX;
    int max_level = INT_MIN;
    for (int i = 1; i < subs_count; i++) {
        if (subs_arr[i]->level < min_level) {
            min_level = subs_arr[i]->level;
        }
        if (subs_arr[i]->level > max_level) {
            max_level = subs_arr[i]->level;
        }
    }

    int levels_count = max_level - min_level + 1;
    uintmax_t* level_cost = (uintmax_t*) calloc(levels_count, sizeof(uintmax_t));

    uintmax_t part1_lowest_cost = UINTMAX_MAX, part2_lowest_cost = UINTMAX_MAX;
    for (int i = 0; i < levels_count; i++) {
        int current_level = min_level + i;
        
        uintmax_t part1_level_cost = (uintmax_t) mb_for_each(submarines, &calc_part_1_cost, &current_level);
        if (part1_level_cost < part1_lowest_cost) part1_lowest_cost = part1_level_cost;

        uintmax_t part2_level_cost = (uintmax_t) mb_for_each(submarines, &calc_part_2_cost, &current_level);
        if (part2_level_cost < part2_lowest_cost) part2_lowest_cost = part2_level_cost;
    }

    printf("Lowest fuel cost for part 1: %llu\n", part1_lowest_cost);
    printf("Lowest fuel cost for part 2: %llu\n", part2_lowest_cost);
}
