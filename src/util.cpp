#include <iostream>
#include <sstream>
#include <cstring>

#include "../include/state.hpp"

#define FEN_CASE(name)     \
	case #name[0]:         \
		state.name |= pos; \
		file++;            \
		break;

State fen(std::string fen) {
	State state = State {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 };

	std::stringstream ss(fen);
	std::string board, move, castling, en_passant, halfmove, fullmove;
	if (!(ss >> board
			 >> move
			 >> castling
			 >> en_passant
			 >> halfmove
			 >> fullmove
	)) {
		std::cerr << "invalid FEN notation\n";
		exit(-1);
	}

	// these are zero indexed
	uint8_t rank = 7; // fen starts at top left
	uint8_t file = 0;

	for (char c : board) {
		// start with a 1 at the LSB, which is A1
		// shift first by rank, then by file
		uint64_t pos = 1ULL << (rank * 8) << file;

		// parse board state
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
				break;

			default:
				std::cerr << "error parsing FEN, found: "  << c << "\n";
				exit(-1);
		};
	}
	
	if (move[0] == 'b') state.metadata = 0b0;

	for (char c : castling) {
		switch (c) {
			case 'k':
				state.metadata |= 0b10;
				break;
			case 'q':
				state.metadata |= 0b100;
				break;
			case 'K':
				state.metadata |= 0b1000;
				break;
			case 'Q':
				state.metadata |= 0b10000;
				break;
			case '-':
				break;
			default:
				printf("unexpected character while parsing casting in FEN");
				exit(-1);
				break;
		}
	}

	// TODO: en passant target square

	state.half_moves = std::stoi(halfmove);
	state.full_moves = std::stoi(fullmove);

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

#define ADD_PIECE(name)             \
	if (board.name & (1ULL << i)) { \
		out[i] = #name[0];          \
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

	// print metadata
	printf("\n");
	if (board.metadata & 1) printf("white to move\n");
	else printf("black to move\n");

	printf("available castling:\n");
	for (int i = 1; i < 5; i++) {
		if (board.metadata & (0b10 << i)) {
			switch (i) {
				case 1:
					printf("white short\n");
					break;
				case 2:
					printf("white long\n");
					break;
				case 3:
					printf("black short\n");
					break;
				case 4:
					printf("black long\n");
					break;
			}
		}
	}

	printf("\n");
	printf("half moves since last capture or pawn move: %d\n", board.half_moves);
	printf("total full moves: %d\n", board.full_moves);
}
