#include <iostream>
#include <cstdint>
#include <cstring>

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

#define FEN_CASE(name)     \
	case #name[0]:       \
		state.name |= pos; \
		file++;            \
		break;

State fen(std::string in) {
	State state = State {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	// these are zero indexed
	uint8_t rank = 7; // fen starts at top left
	uint8_t file = 0;

	for (char c : in) {
		// start with a 1 at the LSB, which is A1
		// shift first by rank, then by file
		uint64_t pos = 1ULL << (rank * 8) << file;
		// printf("%064lb\n", pos);

		switch (c) {
			FEN_CASE(P)
			FEN_CASE(N)
			FEN_CASE(B)
			FEN_CASE(R)
			FEN_CASE(Q)
			FEN_CASE(K)

			FEN_CASE(p)
			FEN_CASE(n)
			FEN_CASE(b)
			FEN_CASE(r)
			FEN_CASE(q)
			FEN_CASE(k)

			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				rank += c - '0';
				break;

			case '/':
				rank--;
				file = 0;
		};
	}

	return state;
}

void print_bitboard(uint64_t bitboard) {
	uint64_t mask = 0b11111111ULL << 56; // a row of ones at the top
	for (int i = 0; i < 8; i++) {
		uint64_t row = bitboard & mask;
		printf("%08lb\n", row >> 56);
		bitboard <<= 8;
	}
}

#define ADD_PIECE(name) 			\
	if (board.name & (1ULL << i)) { \
		out[i] = #name[0]; 			\
	}

void print_board(State board) { 
	char out[64];
	std::memset(out, '.', 64);

	for (int i = 0; i < 64; i++) {
		ADD_PIECE(P)
		ADD_PIECE(N)
		ADD_PIECE(B)
		ADD_PIECE(R)
		ADD_PIECE(Q)
		ADD_PIECE(K)

		ADD_PIECE(p)
		ADD_PIECE(n)
		ADD_PIECE(b)
		ADD_PIECE(r)
		ADD_PIECE(q)
		ADD_PIECE(k)
	}

	for (int i = 0; i < 64; i++) {
		printf("%c", out[i]);
		if ((i + 1) % 8 == 0) printf("\n");
	}
}

int main(void) { 
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

	State start = fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
	print_board(start);
}
