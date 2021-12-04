#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "includes/files.c"

#define MOVE_COMMAND_FORWARD 1
#define MOVE_COMMAND_DOWN 2
#define MOVE_COMMAND_UP 3

typedef struct move_command {
    int direction;
    int distance;
} move;

intptr_t map_to_move(char* line) {
    int direction = 0, distance = 0;
    char* direction_str = (char*) malloc(10*sizeof(char));

    sscanf(line, "%s %d", direction_str, &distance);

    if (strcmp("forward", direction_str) == 0) {
        direction = MOVE_COMMAND_FORWARD;
    } else if (strcmp("down", direction_str) == 0) {
        direction = MOVE_COMMAND_DOWN;
    } else if (strcmp("up", direction_str) == 0) {
        direction = MOVE_COMMAND_UP;
    } else {
        exit(2021);
    }
    free(direction_str);

    move* m = (move*) malloc(sizeof(move*));
    m->direction = direction;
    m->distance = distance;

    return (intptr_t) m;
}

void part1(move** moves, int moves_count) {
    int hor_pos = 0;
    int depth = 0;

    while(--moves_count >= 0) {
        move m = *moves[moves_count];
        if (m.direction == MOVE_COMMAND_FORWARD) {
            hor_pos += m.distance;
        } else {
            depth += m.distance * (m.direction == MOVE_COMMAND_DOWN ? 1 : -1);
        }
    }

    printf("Part 1: %d\n", hor_pos * depth);
}

void part2(move** moves, int moves_count) {
    int hor_pos = 0;
    int depth = 0;
    int aim = 0;

    for (int i = 0; i < moves_count; i++) {
        move m = *moves[i];
        if (m.direction == MOVE_COMMAND_FORWARD) {
            hor_pos += m.distance;
            depth += m.distance * aim;
        } else {
            aim += m.distance * (m.direction == MOVE_COMMAND_DOWN ? 1 : -1);
        }
    }

    printf("Part 2: %d\n", hor_pos * depth);
}

int main() {
    intptr_t* moves = (intptr_t*) malloc(2000 * sizeof(intptr_t));
    int moves_count = map_lines("./day2-input.txt", moves, 2000, &map_to_move);

    part1((move**) moves, moves_count);
    part2((move**) moves, moves_count);
}