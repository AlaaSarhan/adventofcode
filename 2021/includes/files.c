#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 2000

char* read_first_line(char* path) {
    FILE* f = fopen(path, "r");
    char* line = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
    fgets(line, MAX_LINE_LENGTH, f);

    return line;
}

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
            break;
        }

        intptr_t result = (intptr_t) map_fun(line);
        if (!result) {
            continue;
        }

        results[results_count] = result;
        results_count++;

        if (results_count >= max_results) {
            break;
        }
    }

    fclose(f);

    return results_count;
}
