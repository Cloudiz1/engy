#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#define RHMASK 0b01111110ULL
#define RVMASK 0x0001010101010100ULL

namespace rooks {

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

// TODO: this is wrong. when we find an obstruction, we assume we *cant* capture it. wrong.
// also this macro fucking sucks, dont do this shit :sob:
inline std::uint64_t get_moves(int sq, std::uint64_t board) {
    int rank = sq / 8;
    int file = sq % 8;

    #define ROOK_OBS(cond, inc, finc)    \
    {                                    \
        std::uint64_t flag = 0ULL;       \
        std::uint64_t mask = 1ULL << sq; \
        for (int i = 1; cond; i++) {     \
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

inline std::pair<std::vector<uint64_t>, std::vector<uint64_t>>
generate_square(int sq, std::uint64_t mask) {
    std::vector<uint64_t> moves;
    std::vector<uint64_t> boards;

    int count = popcount(mask);
    for (int i = 0; i < (1 << count); i++) {
        std::uint64_t board = create_collision(sq, mask, i);

        boards.push_back(board);
        moves.push_back(get_moves(sq, board));
    }

    return {boards, moves};
}

std::pair<
    std::vector<std::vector<std::uint64_t>>,
    std::vector<std::vector<std::uint64_t>>
>
generate_tables() {
    std::vector<std::vector<std::uint64_t>> boards;
    std::vector<std::vector<std::uint64_t>> moves;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int row = i * 8; // start at bottom row, * 8 squares per row
            int col = 7 - j; // start at left, LSB is left (so larger bitshift at first)

            std::uint64_t mask =
                (RVMASK << col            // align mask on x axis
                 | RHMASK << row)         // align mask on y axis
                & ~(1ULL << (col + row)); // remove the piece itself

            auto [boards_sq, moves_sq] = generate_square(row + col, mask);
            boards.push_back(boards_sq);
            moves.push_back(moves_sq);
        }
    }

    return {boards, moves};
}

std::vector<std::uint64_t> get_masks() {
    std::vector<std::uint64_t> out;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int row = i * 8; // start at bottom row, * 8 squares per row
            int col = 7 - j; // start at left, LSB is left (so larger bitshift at first)

            std::uint64_t mask =
                (RVMASK << col            // align mask on x axis
                 | RHMASK << row)         // align mask on y axis
                & ~(1ULL << (col + row)); // remove the piece itself

            out.push_back(mask);
        }
    }

    return out;
}

size_t lookup(std::uint64_t board, std::uint64_t magic, int n) {
    return (board * magic) >> (64 - n);
}

bool test_square(
    std::vector<std::uint64_t> &boards,
    std::vector<std::uint64_t> &moves,
    std::uint64_t out[4096],
    std::uint64_t magic,
    int n) {
    std::fill(out, out + 4096, std::numeric_limits<std::uint64_t>::max());

    for (int i = 0; i < 64; i++) {
        size_t key = lookup(boards[i], magic, n);

        assert(key < (1ULL << n));

        if (out[key] == std::numeric_limits<std::uint64_t>::max()) {
            out[key] = moves[i];
        } else if (out[key] == moves[i]) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}

void gen_magic() {
    auto [boards, moves] = generate_tables();

    assert(boards.size() == moves.size() && boards.size() == 64);
    assert(boards[0].size() == moves[0].size());
    assert(boards[1].size() == moves[1].size());
    assert(boards[35].size() == moves[35].size());

    long long count = 0;
    for (int i = 0; i < 64; i++) {
        count += boards[i].size();
        count += moves[i].size();
    }

    assert(count == 2 * 102400);

    std::mt19937_64 rng(16180339);
    std::uniform_int_distribution<std::uint64_t> dist;

    std::uint64_t magic_bitboards[64][4096];
	std::uint64_t magic = 0;
    for (;;) {
        magic = dist(rng) & dist(rng) & dist(rng);

		for (int i = 0; i < 64; i++) {
			if (!test_square(boards[i], moves[i], magic_bitboards[i], magic, 12)) {
				continue;
			};
		}

		break;
    }

	printf("%lu\n", magic);
}

} // namespace rooks
