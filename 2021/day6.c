#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "includes/files.c"
#include "includes/memory.c"

static int MEMORY_BUCKET_DEFAULT_SIZE = 20;

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

void advance_age(intptr_t age, intptr_t newgen_fish) {
    if (*((int *)age) == 0) {
        *((int *)age) = 6;

        int* new_age = malloc(sizeof(int));
        *new_age = 8;
        mb_add_element((MEMORY_BUCKET*) newgen_fish, (intptr_t) new_age, MEMORY_BUCKET_DEFAULT_SIZE);
    } else {
        *((int *)age) -= 1;
    }
}

void add_newgen_fish(intptr_t age, intptr_t fish) {
    mb_add_element((MEMORY_BUCKET*) fish, age, MEMORY_BUCKET_DEFAULT_SIZE);
}

void advance_day(MEMORY_BUCKET* fish) {
    MEMORY_BUCKET* newgen_fish = mb_new(MEMORY_BUCKET_DEFAULT_SIZE);

    mb_for_each(fish, &advance_age, (intptr_t) newgen_fish);

    mb_for_each(newgen_fish, &add_newgen_fish, (intptr_t) fish);
}

void solve_old(MEMORY_BUCKET* fish) {
    MEMORY_BUCKET** buckets = (MEMORY_BUCKET**) calloc(100, sizeof(MEMORY_BUCKET*));
    int buckets_count = 0;
    MEMORY_BUCKET* cur = fish;
    while (cur) {
        buckets[buckets_count++] = cur;
        MEMORY_BUCKET* prev = cur;
        cur = cur->next;
        prev->next = NULL;
    }

    int sum_part1 = 0; // count of fish at day 80
    int sum_part2 = 0; // count of fish at day 256

    for (int b = 0; b < buckets_count; b++) {
        for (int d = 0; d < 256; d++) {
            advance_day(buckets[b]);

            if (d == 79) {
                sum_part1 += mb_count_elements(fish);
                // break;
            }
        }

        sum_part2 += mb_count_elements(fish);
        break;

        free(buckets[b]);
    }

    printf("Part 1: %d\n", sum_part1);
    printf("Part 2: %d\n", sum_part2);
}

intmax_t calc_fish_final_count(int age, int days) {
    if (days < 7) {
        return age < (days - 1) ? 2 : 1;
    }

    intmax_t final_count = 1;

    if (age < 6) {
        final_count += calc_fish_final_count(6, days - age - 1 - 2);
        days = days - age - 1;
    }

    int newgen_fish = days / 7;

    for (int lived_weeks = 0; lived_weeks < newgen_fish; lived_weeks++) {
        final_count += calc_fish_final_count(6, (7 * lived_weeks) - 2);
    }

    return final_count;
}

struct results {
    intmax_t day80_count;
    intmax_t day256_count;
};
void sum_final_counts(intptr_t age, intptr_t results) {
    ((struct results*) results)->day80_count += calc_fish_final_count(*((int *)age), 80);
    // ((struct results*) results)->day256_count += calc_fish_final_count(*((int *)age), 256);
}

void solve(MEMORY_BUCKET* fish) {
    struct results results = { .day80_count = 0, .day256_count = 0 };
    mb_for_each(fish, &sum_final_counts, (intptr_t) &results);

    printf("After 80 days: %u\n", results.day80_count);
    printf("After 256 days: %d\n", results.day256_count);
}

void main() {
    char* input = read_first_line("day6-input.txt");
    MEMORY_BUCKET* fish = constrcut_fish(input);
    
    solve(fish);
}
