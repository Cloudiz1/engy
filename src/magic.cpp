#include <cstdint>
#include <map>
#include <random>

#include "../include/util.hpp"

// take a rook bitboard
// iterate over the pieces, for all rooks:
// generate move bitboard (minus certain edges), LAND with blockers
// take a lookup (rook_magic(square, bitstring))
// return a bitstring of precalculated moves

// i think the idea is to generate a map of all possible states (ripple carry) that maps to its possible moves
// *then* try and fit with stronger and stronger magics. ripple carray again, test magic, see if collision is okay

std::map<std::uint64_t, std::uint64_t>
generate_rook_table() {
    // a line of 1s on the right of the board, minus the top and bottom
    std::uint64_t vert_mask = 0x0001010101010100ULL;

    // a line of 1s at the bottom of the board, minus the left and right most
    std::uint64_t horz_mask = 0b01111110ULL;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int row = i * 8; // start at bottom row, * 8 squares per row
            int col = 7 - j; // start at left, LSB is left (so larger bitshift at first)
            std::uint64_t mask =
                (vert_mask << col         // align mask on x axis
                 | horz_mask << row)      // align mask on y axis
                & ~(1ULL << (col + row)); // remove the piece itself

            print_bitboard(mask);
            printf("(%d, %d)\n", i, j);
        }
    }
}

// void gen_rook(std::uint64_t mask, std::uint64_t pos) {
//     uint8_t val;
//     do {
//         val = mask & (0xFF << 8 * i);
//     } while ();
// }

void magic() {
    std::mt19937_64 rng(16180339);
    std::uniform_int_distribution<std::uint64_t> dist;
    std::uint64_t magic = dist(rng) & dist(rng) & dist(rng);
}
