#include "puzzle.h"

#include <cassert>
#include <cstring>
#include <iostream>

namespace {

void test_init_and_clear() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle.size == 4);
    puzzle.cells[0][0] = 3;
    puzzle.top[0] = 2;
    puzzle_clear(&puzzle);
    assert(puzzle.size == 0);
    assert(puzzle.cells[0][0] == 0);
    assert(puzzle.top[0] == 0);

    assert(puzzle_init(nullptr, 4) == -1);
    assert(puzzle_init(&puzzle, 0) == -1);
    assert(puzzle_init(&puzzle, PUZZLE_MAX_SIZE + 1) == -1);
}

void test_load_valid_and_invalid_input() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle, "1--4----3-------", "1234432112344321") == 0);
    assert(puzzle.cells[0][0] == 1);
    assert(puzzle.cells[0][3] == 4);
    assert(puzzle.cells[2][0] == 3);
    assert(puzzle.top[0] == 1);
    assert(puzzle.bottom[3] == 1);
    assert(puzzle.left[0] == 1);
    assert(puzzle.right[3] == 1);

    assert(puzzle_load(&puzzle, "1--x----3-------", "1234432112344321") == -1);
    assert(puzzle_load(&puzzle, "1--5----3-------", "1234432112344321") == -1);
    assert(puzzle_load(&puzzle, "1--4----3-------", "1234z32112344321") == -1);
}

void test_visible_count_edges() {
    int increasing[] = {1, 2, 3, 4};
    int decreasing[] = {4, 3, 2, 1};
    int mixed[] = {2, 1, 4, 3};
    int with_empty[] = {0, 2, 1, 3};

    assert(puzzle_visible_count(increasing, 4) == 4);
    assert(puzzle_visible_count(decreasing, 4) == 1);
    assert(puzzle_visible_count(mixed, 4) == 2);
    assert(puzzle_visible_count(with_empty, 4) == 2);
    assert(puzzle_visible_count(increasing, 0) == 0);
    assert(puzzle_visible_count(nullptr, 4) == -1);
    assert(puzzle_visible_count(increasing, -1) == -1);
}

void test_duplicate_detection() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle, "12-134----------", "0000000000000000") == 0);
    assert(puzzle_has_duplicate_in_row(&puzzle, 0) == 0);
    assert(puzzle_has_duplicate_in_row(&puzzle, 1) == 0);
    assert(puzzle_has_duplicate_in_col(&puzzle, 0) == 1);
    assert(puzzle_has_duplicate_in_col(&puzzle, 1) == 0);

    puzzle.cells[2][2] = 4;
    puzzle.cells[2][3] = 4;
    assert(puzzle_has_duplicate_in_row(&puzzle, 2) == 1);
    assert(puzzle_has_duplicate_in_row(nullptr, 0) == -1);
    assert(puzzle_has_duplicate_in_col(&puzzle, -1) == -1);
    assert(puzzle_has_duplicate_in_col(&puzzle, 4) == -1);
}

void test_clue_validation() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle,
                       "1234"
                       "2143"
                       "3412"
                       "4321",
                       "4221"
                       "1224"
                       "4221"
                       "1224") == 0);

    for (int i = 0; i < 4; i++) {
        assert(puzzle_row_satisfies_clues(&puzzle, i) == 1);
        assert(puzzle_col_satisfies_clues(&puzzle, i) == 1);
    }

    puzzle.left[0] = 3;
    assert(puzzle_row_satisfies_clues(&puzzle, 0) == 0);
    puzzle.left[0] = 4;
    puzzle.cells[0][2] = PUZZLE_EMPTY;
    assert(puzzle_row_satisfies_clues(&puzzle, 0) == -1);
    assert(puzzle_row_satisfies_clues(nullptr, 0) == -1);
    assert(puzzle_col_satisfies_clues(&puzzle, 4) == -1);
}

void test_place_accepts_and_rejects_correctly() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle, "1---------------", "0000000000000000") == 0);

    assert(puzzle_place(&puzzle, 0, 1, 2) == 0);
    assert(puzzle.cells[0][1] == 2);
    assert(puzzle_place(&puzzle, 0, 1, 3) == -1);
    assert(puzzle.cells[0][1] == 2);
    assert(puzzle_place(&puzzle, 0, 2, 2) == -1);
    assert(puzzle.cells[0][2] == PUZZLE_EMPTY);
    assert(puzzle_place(&puzzle, 0, 2, 0) == -1);
    assert(puzzle_place(&puzzle, 0, 2, 5) == -1);
    assert(puzzle_place(nullptr, 0, 2, 3) == -1);
}

void test_place_rejects_completed_clue_violation_and_restores() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle,
                       "124-"
                       "----"
                       "----"
                       "----",
                       "0000"
                       "0000"
                       "4000"
                       "0000") == 0);

    assert(puzzle_place(&puzzle, 0, 3, 3) == -1);
    assert(puzzle.cells[0][3] == PUZZLE_EMPTY);
}

void test_solve_basic_rules() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    puzzle.left[0] = 4;

    int result = puzzle_solve_basic(&puzzle);
    assert(result == 0 || result == 1);
    assert(puzzle.cells[0][0] == 1);
    assert(puzzle.cells[0][1] == 2);
    assert(puzzle.cells[0][2] == 3);
    assert(puzzle.cells[0][3] == 4);

    assert(puzzle_init(&puzzle, 4) == 0);
    puzzle.top[1] = 1;
    result = puzzle_solve_basic(&puzzle);
    assert(result == 0 || result == 1);
    assert(puzzle.cells[0][1] == 4);

    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle,
                       "123-"
                       "----"
                       "----"
                       "----",
                       "0000000000000000") == 0);
    result = puzzle_solve_basic(&puzzle);
    assert(result == 0 || result == 1);
    assert(puzzle.cells[0][3] == 4);
    assert(puzzle_solve_basic(nullptr) == -1);
}

} // namespace

int main() {
    test_init_and_clear();
    test_load_valid_and_invalid_input();
    test_visible_count_edges();
    test_duplicate_detection();
    test_clue_validation();
    test_place_accepts_and_rejects_correctly();
    test_place_rejects_completed_clue_violation_and_restores();
    test_solve_basic_rules();
    std::cout << "hw01 tests passed\n";
    return 0;
}
