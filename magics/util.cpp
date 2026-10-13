#include <cstdint>
#include <stdio.h>

void print_bitboard(uint64_t bitboard) {
    uint64_t mask = 0b11111111ULL << 56; // a row of ones at the top
    for (int i = 0; i < 8; i++) {
        uint64_t row = bitboard & mask;
        printf("%08lb\n", row >> 56);
        bitboard <<= 8;
    }

    printf("\n");
}
