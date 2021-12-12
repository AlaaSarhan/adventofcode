#include <stdlib.h>
#include <stdio.h>
#include "includes/files.c"

typedef struct geoline_struct {
    int x1;
    int y1;
    int x2;
    int y2;
} GeoLine;

intptr_t map_to_geoline(char* line) {
    GeoLine* geoline = (GeoLine*)malloc(sizeof(GeoLine));
    sscanf(
        line,
        "%d,%d -> %d,%d",
        &geoline->x1,
        &geoline->y1,
        &geoline->x2,
        &geoline->y2
    );

    return (intptr_t) geoline;
}

void print_canvas(int** canvas1) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf(" %2d ", canvas1[i][j]);
        }
        printf("\n");
    }
}

void mark_line_on_canvas(int** canvas1, GeoLine* geoline) {
    GeoLine origin_and_vector = {
        // Origin
        .x1 = geoline->x1,
        .y1 = geoline->y1,
        // Vector
        .x2 = geoline->x2 - geoline->x1,
        .y2 = geoline->y2 - geoline->y1
    };

    canvas1[origin_and_vector.x1][origin_and_vector.y1] += 1;
    while(origin_and_vector.x2 != 0 || origin_and_vector.y2 != 0) {
        canvas1
            [origin_and_vector.x1 + origin_and_vector.x2]
            [origin_and_vector.y1 + origin_and_vector.y2] += 1;

        origin_and_vector.x2 -= (origin_and_vector.x2 > 0) - (origin_and_vector.x2 < 0);
        origin_and_vector.y2 -= (origin_and_vector.y2 > 0) - (origin_and_vector.y2 < 0);
    }
}

void solve(GeoLine** geolines, int geolines_count) {
    int** canvas1 = (int**) malloc(sizeof(int*) * 1000);
    for (int i = 0; i < 1000; i++) {
        canvas1[i] = (int*) calloc(1000, sizeof(int));
    }
    int** canvas2 = (int**) malloc(sizeof(int*) * 1000);
    for (int i = 0; i < 1000; i++) {
        canvas2[i] = (int*) calloc(1000, sizeof(int));
    }

    for (int l = 0; l < geolines_count; l++) {
        GeoLine* geoline = geolines[l];

        if (geoline->x1 == geoline->x2 || geoline->y1 == geoline->y2) {
            mark_line_on_canvas(canvas1, geoline);
        }

        mark_line_on_canvas(canvas2, geoline);
    }

    int dangerous_points_sum1 = 0;
    int dangerous_points_sum2 = 0;
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            if (canvas1[i][j] > 1) {
                dangerous_points_sum1++;
            }
            if (canvas2[i][j] > 1) {
                dangerous_points_sum2++;
            }
        }
    }
    printf("Part 1: %d\n", dangerous_points_sum1);
    printf("Part 2: %d\n", dangerous_points_sum2);
}

void main() {
    intptr_t* geolines = (intptr_t*) malloc(1000 * sizeof(intptr_t));
    int geolines_count = map_lines("day5-input.txt", geolines, 1000, &map_to_geoline);

    solve((GeoLine**) geolines, geolines_count);
}