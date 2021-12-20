#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "includes/files.c"
#include "includes/memory.c"

static int MEMORY_BUCKET_DEFAULT_SIZE = 100;

typedef struct submarine {
    int level;
    uintmax_t total_cost;
} SUBMARINE;

intptr_t update_cost(intptr_t element, intptr_t extra, intptr_t _) {
    SUBMARINE* new_submarine = (SUBMARINE*) extra;
    SUBMARINE* submarine = (SUBMARINE*) element;
    uintmax_t additional_cost = abs(submarine->level - new_submarine->level);
    submarine->total_cost += additional_cost;
    new_submarine->total_cost += additional_cost;
    return _;
}

MEMORY_BUCKET* constrcut_levels(char* input) {
    char* crab_level = strtok(input, ",");

    SUBMARINE* element = (SUBMARINE*) malloc(sizeof(SUBMARINE));
    element->level = atoi(crab_level);
    element->total_cost = 0;
    MEMORY_BUCKET* submarines = mb_add_element(NULL, (intptr_t) element, MEMORY_BUCKET_DEFAULT_SIZE);

    while (crab_level = strtok(NULL, ",")) {
        SUBMARINE* element = (SUBMARINE*) malloc(sizeof(SUBMARINE));
        element->level = atoi(crab_level);
        element->total_cost = 0;
        mb_add_element(submarines, (intptr_t) element, MEMORY_BUCKET_DEFAULT_SIZE);

        mb_for_each(submarines, &update_cost, (intptr_t) &element->level);
    }

    return submarines;
}

void main() {
    char* input = read_first_line("day7-input.txt");
    MEMORY_BUCKET* submarines = constrcut_levels(input);

    uintmax_t subs_count;
    SUBMARINE** subs_arr = (SUBMARINE**) mb_to_array(submarines, &subs_count);

    SUBMARINE* selectedSubmarine = subs_arr[0];
    for (int i = 1; i < subs_count; i++) {
        if (subs_arr[i]->total_cost <= selectedSubmarine->total_cost) {
            selectedSubmarine = subs_arr[i];
        }
    }
    printf("Level with lowest cost (%llu): %d\n", selectedSubmarine->total_cost, selectedSubmarine->level);
}