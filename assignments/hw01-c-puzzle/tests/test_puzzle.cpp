#include "puzzle.h"

#include <cassert>
#include <cstring>
#include <iostream>

static void test_load() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle, "1--4----3-------", "1234432112344321") == 0);
    assert(puzzle.cells[0][0] == 1);
    assert(puzzle.cells[0][3] == 4);
    assert(puzzle.top[0] == 1);
    assert(puzzle.bottom[3] == 1);
}

static void test_visible_count() {
    int values[] = {1, 3, 2, 4};
    assert(puzzle_visible_count(values, 4) == 3);
}

static void test_duplicates() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle, "12-134----------", "0000000000000000") == 0);
    assert(puzzle_has_duplicate_in_row(&puzzle, 0) == 0);
    assert(puzzle_has_duplicate_in_col(&puzzle, 0) == 1);
}

static void test_place_rejects_bad_move() {
    puzzle_t puzzle{};
    assert(puzzle_init(&puzzle, 4) == 0);
    assert(puzzle_load(&puzzle, "1---------------", "0000000000000000") == 0);
    assert(puzzle_place(&puzzle, 0, 1, 1) == -1);
    assert(puzzle.cells[0][1] == PUZZLE_EMPTY);
}

int main() {
    test_load();
    test_visible_count();
    test_duplicates();
    test_place_rejects_bad_move();
    std::cout << "hw01 tests passed\n";
    return 0;
}
