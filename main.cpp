#include "moves/moves.cpp"
#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.cpp"
#include "structure/board.h"
#include "structure/square.h"
#include <iostream>

using namespace chess_engine;

void print_bitboard(bit::Bitboard b) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = rank * 8 + file;
            std::cout << (bit::test_bit(b, square) ? "1 " : ". ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    board::Board board = board::set_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    std::cout << board.to_string() << std::endl;

    return 0;
}
