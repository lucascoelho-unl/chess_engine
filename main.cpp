#include "bitboard.h"
#include "board.cpp"
#include <iostream>

void print_bitboard(Bitboard b) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << (test_bit(b, square) ? "1 " : ". ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    Board board = set_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    board.create_white_pawn("e2", "e4");

    std::cout << board.to_string() << std::endl;

    return 0;
}
