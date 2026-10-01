#pragma once
#include <cstdint>
#include <random>

#include "../include/state.hpp"

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

	std::uint64_t hash(State& state) {
	}
};
