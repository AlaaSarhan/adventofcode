#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1000

int parse_lines_as_int(char* path, int* lines, int max_lines) {
    FILE* f = fopen(path, "r");

    int lines_count = 0;
    char* line;

    while (1) {
        line = malloc(MAX_LINE_LENGTH * sizeof(char));
        fgets(line, MAX_LINE_LENGTH, f) != NULL;

        if (line == NULL) {
            free(line);
            break;
        }

        lines[lines_count] = atoi(line);
        lines_count++;

        if (lines_count >= max_lines) {
            break;
        }
    }

    fclose(f);

    return lines_count;
}