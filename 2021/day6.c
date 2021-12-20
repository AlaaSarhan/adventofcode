#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "includes/files.c"
#include "includes/memory.c"

static int MEMORY_BUCKET_DEFAULT_SIZE = 100;

MEMORY_BUCKET* constrcut_fish(char* input) {
    char* fish_age = strtok(input, ",");

    int* age = (int*) malloc(sizeof(int));
    *age = atoi(fish_age);
    MEMORY_BUCKET* fish = mb_add_element(NULL, (intptr_t) age, MEMORY_BUCKET_DEFAULT_SIZE);

    while (fish_age = strtok(NULL, ",")) {
        int* age = (int*) malloc(sizeof(int));
        *age = atoi(fish_age);
        mb_add_element(fish, (intptr_t) age, MEMORY_BUCKET_DEFAULT_SIZE);
    }

    return fish;
}

intptr_t sum_by_age(intptr_t element, intptr_t extra, intptr_t accumulator) {
    ((uintmax_t*) extra)[*((int*) element)]++;
    return accumulator;
}

uintmax_t sum_fish(uintmax_t* counts_by_age) {
    uintmax_t sum = 0;
    for (int i = 0; i < 9; i++) {
        sum += counts_by_age[i];
    }

    return sum;
}

void solve(MEMORY_BUCKET* fish) {
    uintmax_t counts_by_age[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    mb_for_each(fish, &sum_by_age, (intptr_t) counts_by_age);

    for (int i = 0; i < 256; i++) {
        uintmax_t new_8 = counts_by_age[0];
        uintmax_t new_6 = counts_by_age[0] + counts_by_age[7];
        for (int j = 0; j < 6; j++) {
            counts_by_age[j] = counts_by_age[j+1];
        }
        counts_by_age[7] = counts_by_age[8];
        counts_by_age[6] = new_6;
        counts_by_age[8] = new_8;

        if (i == 79) {
            printf("Day 80: %llu\n", sum_fish(counts_by_age));
        }
    }
    printf("Day 256: %llu\n", sum_fish(counts_by_age));
}

void main() {
    char* input = read_first_line("day6-input.txt");
    MEMORY_BUCKET* fish = constrcut_fish(input);
    
    solve(fish);
}
