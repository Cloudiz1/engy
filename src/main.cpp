#include <cstring>
#include <iostream>

#include "../include/state.hpp"
#include "../include/util.hpp"

int main(void) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    State start = fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    print_board(start);
}
