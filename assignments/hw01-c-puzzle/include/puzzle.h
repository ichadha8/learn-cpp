#ifndef TRAINING_HW01_PUZZLE_H
#define TRAINING_HW01_PUZZLE_H

#ifdef __cplusplus
extern "C" {
#endif

#define PUZZLE_MAX_SIZE 8
#define PUZZLE_EMPTY 0

typedef struct {
    int size;
    int cells[PUZZLE_MAX_SIZE][PUZZLE_MAX_SIZE];
    int top[PUZZLE_MAX_SIZE];
    int bottom[PUZZLE_MAX_SIZE];
    int left[PUZZLE_MAX_SIZE];
    int right[PUZZLE_MAX_SIZE];
} puzzle_t;

int puzzle_init(puzzle_t *puzzle, int size);
int puzzle_load(puzzle_t *puzzle, const char *state, const char *clues);
void puzzle_clear(puzzle_t *puzzle);

int puzzle_visible_count(const int *values, int count);
int puzzle_has_duplicate_in_row(const puzzle_t *puzzle, int row);
int puzzle_has_duplicate_in_col(const puzzle_t *puzzle, int col);
int puzzle_row_satisfies_clues(const puzzle_t *puzzle, int row);
int puzzle_col_satisfies_clues(const puzzle_t *puzzle, int col);
int puzzle_place(puzzle_t *puzzle, int row, int col, int value);
int puzzle_solve_basic(puzzle_t *puzzle);

#ifdef __cplusplus
}
#endif

#endif
