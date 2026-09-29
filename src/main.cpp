#include <iostream>
#include <cstdint>

// bitboards for all 12 pieces. LSB is A1
// follows fen notation:
// lowercase for white, upper for black
// p -> pawn
// n -> knight
// b -> bishop
// r -> rook
// q -> queen
// k -> king
struct State {
	uint64_t p;
	uint64_t n;
	uint64_t b;
	uint64_t r;
	uint64_t q;
	uint64_t k;

	uint64_t P;
	uint64_t N;
	uint64_t B;
	uint64_t R;
	uint64_t Q;
	uint64_t K;
};

State fen(std::string in) {
	State state = State {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// these are zero indexed
	uint8_t rank = 7; // fen starts at top left
	uint8_t file = 0;

	// start with a 1 at the LSB, which is A1
	// shift first by rank, then by file
	uint64_t pos = 1 << (rank * 8) << file;

	for (char c : in) {
		switch (c) {
			case 'P':
			case 'N':
			case 'B':
			case 'R':
			case 'Q':
			case 'K':

			case 'p':
			case 'n':
			case 'b':
			case 'r':
			case 'q':
			case 'k':

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':

			case '/':
				rank--;
				file = 1;
		};
	}
}

void print_board(uint64_t bitboard) {
	uint64_t mask = 0b11111111ULL << 56; // a row of ones at the top
	for (int i = 0; i < 8; i++) {
		uint64_t row = bitboard & mask;
		printf("%08lb\n", row >> 56);
		bitboard <<= 8;
	}
}

int main(void) {
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(NULL);
	print_board(0xFF00FF00FF00FF00);
}
