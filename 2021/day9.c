#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "includes/files.c"

#define Z 10
#define X 100
#define Y 100

void main() {
    bool terrain[Z][X][Y][2]; // z, x, y => [true, true] if low-point, [true, false] if not low-point, [false, false] if no point
    for (int z = 0; z < Z; z++) {
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                terrain[z][x][y][0] = false; // no terrain
                terrain[z][x][y][1] = false; // not low-point
            }
        }
    }
    
    char* line;
    int y = 0;
    for (
        FILE *f = NULL;
        line = read_line("day9-input.txt", &f);
        y++
    ) {
        size_t line_len = strlen(line);
        for (int x = 0; x < line_len; x++) {
            int z = line[x] - '0';

            terrain[z][x][y][0] = true;
            
            bool has_lower_neighbour = false;
            for (int lower = z - 1; lower >= 0; lower--) {
                if (
                    (x < X - 1 && terrain[lower][x+1][y][0]) ||
                    (x > 0 && terrain[lower][x-1][y][0]) ||
                    (y < Y - 1 && terrain[lower][x][y+1][0]) ||
                    (y > 0 && terrain[lower][x][y-1][0])
                ) {
                    has_lower_neighbour = true;
                    break;
                }
            }

            for (int higher = z; higher < 10; higher++) {
                if (x < X - 1) terrain[higher][x+1][y][1] = false;
                if (x > 0) terrain[higher][x-1][y][1] = false;
                if (y < Y - 1) terrain[higher][x][y+1][1] = false;
                if (y > 0) terrain[higher][x][y-1][1] = false;
            }

            if (has_lower_neighbour) {
                continue;
            }

            terrain[z][x][y][1] = true;
        }
    }

    int total_risk = 0;
    for (int z = 0; z < Z; z++) {
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                if (terrain[z][x][y][1]) {
                    printf("counting %d, %d (%d) \n", x, y, z);
                    total_risk += z + 1;
                }
            }
        }
    }

    printf("Part 1: %d\n", total_risk);
}
