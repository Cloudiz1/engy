#pragma once
#include <cstdint>
#include <random>

#include "../include/state.hpp"

#define PIECES \
    X(P, 0)    \
    X(N, 1)    \
    X(B, 2)    \
    X(R, 3)    \
    X(Q, 4)    \
    X(K, 5)    \
    X(p, 6)    \
    X(n, 7)    \
    X(b, 8)    \
    X(r, 9)    \
    X(q, 10)   \
    X(k, 11)

struct Zobrist {
    // 12 pieces by 64 squares
    std::uint64_t pieces[12][64];

    // XOR if white to move, nothing if black to move
    std::uint64_t wtm;

    // order is a 4 bit string: kqKQ (grabbed by `state.metadata & 0x0F`)
    std::uint64_t castling[16];

    // en passant file, 0-7
    std::uint64_t epf[8];

    Zobrist() {
        std::mt19937_64 rng(16180339);
        std::uniform_int_distribution<std::uint64_t> dist;

        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 64; j++) {
                pieces[i][j] = dist(rng);
            }
        }

        for (int i = 0; i < 16; i++) {
            castling[i] = dist(rng);
        }

        for (int i = 0; i < 8; i++) {
            castling[i] = dist(rng);
        }

        wtm = dist(rng);
    }

    std::uint64_t hash(State &state) {
        std::uint64_t hash = 0b0ULL;
		// hashes ALL pieces
        // clang-format off
		#define X(piece, pindex)               \
			for (int i = 0; i < 64; i++) {     \
				if (state.piece & (1 << i)) {  \
					hash ^= pieces[pindex][i]; \
				}                              \
			}

		PIECES
		#undef X
        // clang-format on

		hash ^= castling[state.metadata & 0xF];

		if (state.metadata & (1 << 4)) {
			hash ^= wtm;
		}

		if (state.ep != 0) {
			for (int i = 0; i < 8; i++) {
				if (state.ep & (1 << i)) {
					hash ^= epf[i];
					break;
				}
			}
		}

        return hash;
    }
};
