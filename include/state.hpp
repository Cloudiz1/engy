#pragma once
#include <cstdint>

// bitboards for all 12 pieces. LSB is A1
// follows fen notation:
// lowercase for white, upper for black
// p: pawn
// n: knight
// b: bishop
// r: rook
// q: queen
// k: king
struct State {
	std::uint64_t p;
	std::uint64_t n;
	std::uint64_t b;
	std::uint64_t r;
	std::uint64_t q;
	std::uint64_t k;

	std::uint64_t P;
	std::uint64_t N;
	std::uint64_t B;
	std::uint64_t R;
	std::uint64_t Q;
	std::uint64_t K;

	std::uint64_t obstructions;

	// LSB to MSB:
	// can white short castle
	// can white long castle
	// can black short castle
	// can black long castle
	// white to move
	std::uint8_t metadata;

	std::uint8_t ep;
	
	// half moves since last capture
	std::uint8_t half_moves;

	// total full moves
	std::uint8_t full_moves;
};
