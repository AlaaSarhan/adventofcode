#include <stdio.h>
#include <stdlib.h>
#include "includes/files.c"

void part1(int* lines, int lines_count) {
    if (lines_count < 2) {
        printf("N/A");
    }

    int previous_measurement = *lines;
    int increases = 0;

    while(lines_count > 1) {
        lines++; lines_count--;

        int current_measurement = *lines;
        if (current_measurement > previous_measurement) {
            increases++;
        }
        previous_measurement = current_measurement;
    }

    printf("Part 1: %d\n", increases);
}

void part2(int* lines, int lines_count) {
    if (lines_count < 4) {
        printf("N/A");
    }

    int previous_sum = lines[0] + lines[1] + lines[2];
    int increases = 0;
    lines_count -= 3;
    lines += 3;

    while(lines_count > 0) {
        int current_sum = previous_sum + *lines - *(lines - 3);
        if (current_sum > previous_sum) {
            increases++;
        }
        previous_sum = current_sum;

        lines++;
        lines_count--;
    }
    
    printf("Part 2: %d\n", increases);
}

int main() {
    int* lines = (int*) malloc(10000 * sizeof(void *));
    int lines_count = parse_lines_as_int("./day1-input.txt", lines, 10000);

    part1(lines, lines_count);
    part2(lines, lines_count);
}
