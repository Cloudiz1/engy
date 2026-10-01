#pragma once
#include <string>
#include "state.hpp"

State fen(std::string fen);
void print_bitboard(uint64_t bitboard);
void print_board(State board);
