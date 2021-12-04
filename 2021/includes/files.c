#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 1000

int map_lines(
    char* path,
    intptr_t* results,
    int max_results,
    intptr_t (*map_fun)(char* line)
) {
    FILE* f = fopen(path, "r");

    int results_count = 0;
    char* line;

    while (1) {
        line = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));

        if (fgets(line, MAX_LINE_LENGTH, f) == NULL) {
            free(line);
            break;
        }

        results[results_count] = (intptr_t) map_fun(line);
        results_count++;

        if (results_count >= max_results) {
            break;
        }
    }

    fclose(f);

    return results_count;
}