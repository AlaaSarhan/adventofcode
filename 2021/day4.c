#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "includes/files.c"

#define     BOARD_SIZE       5

typedef struct board_struct {
    int** cells;
    int* row_sums;
    int* col_sums;
    int* row_markings;
    int winning_number;
} board;

void print_board(board* board) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int mark = 1 << c;
            if (mark & board->row_markings[r]) {
                printf(" [%2d] ", board->cells[r][c]);
            } else {
                printf("  %2d  ", board->cells[r][c]);
            }
        }
        printf("\n");
    }
    printf("winning number: %d\n\n", board->winning_number);
}

int numbers_count = 0;
board* current_board = NULL;
int** current_row = NULL;
intptr_t build_input_and_boards(char* line) {
    if (numbers_count == 0) {
        int* numbers = (int*) calloc(400, sizeof(int));
        
        char* number = strtok(line, ",");
        do {
            numbers[numbers_count++] = atoi(number);
        } while(number = strtok(NULL, ","));

        return (intptr_t) numbers;
    } else if (strlen(line) <= 2) {
        intptr_t return_board = 0;
        if (current_board != NULL) {
            return_board = (intptr_t) current_board;
        }

        current_board = (board*) calloc(1, sizeof(board));
        current_board->col_sums = (int*) calloc(BOARD_SIZE, sizeof(int));
        current_board->row_sums = (int*) calloc(BOARD_SIZE, sizeof(int));
        current_board->row_markings = (int*) calloc(BOARD_SIZE, sizeof(int));
        current_board->cells = (int**) calloc(BOARD_SIZE, sizeof(int*));
        current_board->winning_number = -1;
        for (int i = 0; i < BOARD_SIZE; i++) {
            current_board->cells[i] = (int*) calloc(BOARD_SIZE, sizeof(int));
        }
        current_row = current_board->cells;

        return return_board;
    } else {
        int* current_cell = current_row[0];

        char* number = strtok(line, " ");
        do {
            *current_cell = atoi(number);
            current_cell++;
        } while(number = strtok(NULL, " "));
        current_row++;

        return 0;
    }
}

int calc_board_score(board* board) {
    int unmarked_sum = 0;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int mark = 1 << c;
            if (mark & board->row_markings[r]) {
                continue;
            }
            unmarked_sum += board->cells[r][c];
        }
    }
    return board->winning_number * unmarked_sum;
}

void solve(int* numbers, int numbers_count, board** boards, int boards_count) {
    board* last_winning_board = NULL;
    for (int i = 0; i < numbers_count; i++) {
        int number = numbers[i];
        for (int j = 0; j < boards_count; j++) {
            board* board = boards[j];

            for (int r = 0; board->winning_number < 0 && r < BOARD_SIZE; r++) {
                for (int c = 0; board->winning_number < 0 && c < BOARD_SIZE; c++) {
                    int cell = board->cells[r][c];

                    if (cell == number) {
                        int mark = 1 << c;
                        board->row_markings[r] |= mark;

                        board->col_sums[c] += 1;
                        board->row_sums[r] += 1;

                        if (board->col_sums[c] == BOARD_SIZE ||
                            board->row_sums[r] == BOARD_SIZE
                        ) {
                            board->winning_number = number;
                            if (last_winning_board == NULL) {
                                int score = calc_board_score(board);
                                printf("Part 1: %d\n", score);
                            }
                            last_winning_board = board;
                            
                            print_board(board);
                        }
                    }
                }
            }
        }
    }

    if (last_winning_board) {
        int score = calc_board_score(last_winning_board);
        printf("Part 2: %d\n", score);
    }
}

void main() {
    intptr_t* results = (intptr_t*) malloc(200 * sizeof(intptr_t));
    int results_count = map_lines("day4-input.txt", results, 200, &build_input_and_boards);

    int* numbers = (int*) results[0];
    board** boards = (board**) (results + 1);
    int boards_count = results_count - 1;

    solve(numbers, numbers_count, boards, boards_count);
}