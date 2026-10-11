#include <cassert>
#include <cstdint>
#include <map>
#include <random>
#include <vector>

#define RHMASK 0b01111110ULL
#define RVMASK 0x0001010101010100ULL

#define BITBOARD(                     \
    r8, r7, r6, r5, r4, r3, r2, r1) ( \
    ((uint64_t)(r8) << 56) |          \
    ((uint64_t)(r7) << 48) |          \
    ((uint64_t)(r6) << 40) |          \
    ((uint64_t)(r5) << 32) |          \
    ((uint64_t)(r4) << 24) |          \
    ((uint64_t)(r3) << 16) |          \
    ((uint64_t)(r2) << 8) |           \
    ((uint64_t)(r1) << 0))

#define DEBUG0 BITBOARD( \
    0b00000000,          \
    0b00000000,          \
    0b00000000,          \
    0b00000000,          \
    0b00000000,          \
    0b00000000,          \
    0b00000000,          \
    0b00000000)

#define DEBUG1 BITBOARD( \
    0b00000000,          \
    0b10000000,          \
    0b10000000,          \
    0b00000000,          \
    0b10000000,          \
    0b10000000,          \
    0b10000000,          \
    0b01110110)

#define DEBUG2 BITBOARD( \
    0b01101110,          \
    0b00000001,          \
    0b00000001,          \
    0b00000001,          \
    0b00000000,          \
    0b00000001,          \
    0b00000001,          \
    0b00000000)

#define DEBUG3 BITBOARD( \
    0b00000000,          \
    0b00010000,          \
    0b00000000,          \
    0b01000100,          \
    0b00000000,          \
    0b00000000,          \
    0b00010000,          \
    0b00000000)

#include "../include/util.hpp"

// take a rook bitboard
// iterate over the pieces, for all rooks:
// generate move bitboard (minus certain edges), LAND with blockers
// take a lookup (rook_magic(square, bitstring))
// return a bitstring of precalculated moves

// i think the idea is to generate a map of all possible states (ripple carry) that maps to its possible moves
// *then* try and fit with stronger and stronger magics. ripple carray again, test magic, see if collision is okay

int popcount(std::uint64_t bitstring) {
    int count = 0;
    for (std::uint64_t i = 0; i < 64; i++) {
        if (bitstring & (1ULL << i)) {
            count++;
        }
    }

    // highest bitmask number is 12: a rook in a corner
    assert(count <= 12);
    return count;
}

inline std::uint64_t create_collision(int sq, std::uint64_t mask, int collisions) {
    int original = popcount(mask);

    int collision_ptr = 0;
    for (uint64_t i = 0; i < 64; i++) {
        if (mask & (1ULL << i)) {
            std::uint64_t collision_bit = (collisions & (1 << collision_ptr)) >> collision_ptr;
            mask &= ~(collision_bit << i);
            collision_ptr++;
        }
    }

    assert(collision_ptr <= 12);

    // if collisions is a power of two, there should only be a single bit removed
    // thus, popcount(mask) == popcount(oriignal) - 1
    if ((collisions & (collisions - 1)) == 0 && collisions > 0) {
        assert(popcount(mask) == original - 1);
    }

    return mask;
}

inline std::uint64_t gen_rook_move(int sq, std::uint64_t board) {
    int rank = sq / 8;
    int file = sq % 8;

    #define ROOK_OBS(cond, inc, finc)    \
    {                                    \
        std::uint64_t flag = 0ULL;       \
        std::uint64_t mask = 1ULL << sq; \
        for (int i = 1; cond; i++) { \
            flag inc;                    \
            mask inc;                    \
            if (!(board & mask))         \
                flag = 1ULL << sq finc;  \
            board &= ~(flag);            \
        }                                \
    }

    ROOK_OBS(i < rank, >>= 8, >> i * 8)
    ROOK_OBS(i < 7 - rank, <<= 8, << i * 8)
    ROOK_OBS(i < file, >>= 1, >> i)
    ROOK_OBS(i < 7 - file, <<= 1, << i)

    return board;
}

inline std::vector<uint64_t> generate_square(int sq, std::uint64_t mask) {
    std::vector<uint64_t> out;
    int count = popcount(mask);
    for (int i = 0; i < (1 << count); i++) {
        std::uint64_t new_board = create_collision(sq, mask, i);
        std::uint64_t move_set = gen_rook_move(sq, new_board);

		switch (new_board) {
			case DEBUG1: 
				print_bitboard(move_set);
				break;
			case DEBUG2: 
				print_bitboard(move_set);
				break;
			case DEBUG3: 
				print_bitboard(move_set);
				break;
		}
    }

    return out;
}

std::map<std::uint64_t, std::uint64_t>
generate_tables() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int row = i * 8; // start at bottom row, * 8 squares per row
            int col = 7 - j; // start at left, LSB is left (so larger bitshift at first)
            std::uint64_t mask =
                (RVMASK << col            // align mask on x axis
                 | RHMASK << row)         // align mask on y axis
                & ~(1ULL << (col + row)); // remove the piece itself

            // print_bitboard(mask);
            // printf("(%d, %d)\n", i, j);

            generate_square(row + col, mask);
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
