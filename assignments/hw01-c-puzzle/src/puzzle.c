#include "puzzle.h"

#include <stddef.h>
#include <string.h>

static int valid_size(int size) {
    return size >= 1 && size <= PUZZLE_MAX_SIZE;
}

static int char_to_cell(char c) {
    if (c == '-' || c == '0') {
        return PUZZLE_EMPTY;
    }
    if (c >= '1' && c <= '8') {
        return c - '0';
    }
    return -1;
}

int puzzle_init(puzzle_t *puzzle, int size) {
    if (puzzle == NULL || !valid_size(size)) {
        return -1;
    }

    memset(puzzle, 0, sizeof *puzzle);
    puzzle->size = size;
    return 0;
}

void puzzle_clear(puzzle_t *puzzle) {
    if (puzzle == NULL) {
        return;
    }
    memset(puzzle, 0, sizeof *puzzle);
}

int puzzle_load(puzzle_t *puzzle, const char *state, const char *clues) {
    if (puzzle == NULL || state == NULL || clues == NULL || !valid_size(puzzle->size)) {
        return -1;
    }

    int size = puzzle->size;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            int value = char_to_cell(state[row * size + col]);
            if (value < 0 || value > size) {
                return -1;
            }
            puzzle->cells[row][col] = value;
        }
    }

    for (int i = 0; i < size; i++) {
        int top = char_to_cell(clues[i]);
        int bottom = char_to_cell(clues[size + i]);
        int left = char_to_cell(clues[(2 * size) + i]);
        int right = char_to_cell(clues[(3 * size) + i]);
        if (top < 0 || bottom < 0 || left < 0 || right < 0) {
            return -1;
        }
        puzzle->top[i] = top;
        puzzle->bottom[i] = bottom;
        puzzle->left[i] = left;
        puzzle->right[i] = right;
    }

    return 0;
}

int puzzle_visible_count(const int *values, int count) {
    (void)values;
    (void)count;
    return -1; /* TODO */
}

int puzzle_has_duplicate_in_row(const puzzle_t *puzzle, int row) {
    (void)puzzle;
    (void)row;
    return -1; /* TODO */
}

int puzzle_has_duplicate_in_col(const puzzle_t *puzzle, int col) {
    (void)puzzle;
    (void)col;
    return -1; /* TODO */
}

int puzzle_row_satisfies_clues(const puzzle_t *puzzle, int row) {
    (void)puzzle;
    (void)row;
    return -1; /* TODO */
}

int puzzle_col_satisfies_clues(const puzzle_t *puzzle, int col) {
    (void)puzzle;
    (void)col;
    return -1; /* TODO */
}

int puzzle_place(puzzle_t *puzzle, int row, int col, int value) {
    (void)puzzle;
    (void)row;
    (void)col;
    (void)value;
    return -1; /* TODO */
}

int puzzle_solve_basic(puzzle_t *puzzle) {
    (void)puzzle;
    return -1; /* TODO */
}
