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

	// LSB to MSB:
	// white to move
	// can white short castle
	// can white long castle
	// can black short castle
	// can black long castle
	std::uint8_t metadata;
	
	// half moves since last capture
	std::uint8_t half_moves;

	// total full moves
	std::uint8_t full_moves;
};
