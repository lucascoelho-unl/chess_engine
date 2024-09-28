#include "utils.h"
#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

namespace utils {

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

piece::Color opposite_color(piece::Color c) {
    return (c == piece::Color::WHITE) ? piece::Color::BLACK : piece::Color::WHITE;
}

} // namespace utils