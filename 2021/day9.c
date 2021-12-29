#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "includes/files.c"
#include "includes/memory.c"

#define Z 10
#define X 100
#define Y 100

uintmax_t scan_basin(bool wall[X][Y][2], bool visited[X][Y], int x, int y) {
    visited[x][y] = true;

    return 1 +
        ((x > 0 && !wall[x-1][y][0] && !visited[x-1][y]) ? scan_basin(wall, visited, x-1, y) : 0) +
        ((x < X - 1 && !wall[x+1][y][0] && !visited[x+1][y]) ? scan_basin(wall, visited, x+1, y) : 0) +
        ((y > 0 && !wall[x][y-1][0] && !visited[x][y-1]) ? scan_basin(wall, visited, x, y-1) : 0) +
        ((y < Y - 1 && !wall[x][y+1][0] && !visited[x][y+1]) ? scan_basin(wall, visited, x, y+1) : 0);
}

int comapre_basins(const void *basin1, const void *basin2) {
    uintmax_t b1 = (uintmax_t) *((uintmax_t*) basin1);
    uintmax_t b2 = (uintmax_t) *((uintmax_t*) basin2);
    return b2 - b1;
}

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

    uintmax_t basin_sizes[526];
    int basins_count = 0;
    int total_risk = 0;
    bool visited[X][Y];
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            visited[x][y] = false;
        }
    }
    for (int z = 0; z < Z; z++) {
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                if (terrain[z][x][y][1]) {
                    total_risk += z + 1;

                    basin_sizes[basins_count++] = scan_basin(terrain[9], visited, x, y);
                }
            }
        }
    }

    qsort(basin_sizes, basins_count, sizeof(uintmax_t), &comapre_basins);

    printf("Part 1: %d\n", total_risk);
    printf("Part 2: %lld\n", basin_sizes[0] * basin_sizes[1] * basin_sizes[2]);
}
