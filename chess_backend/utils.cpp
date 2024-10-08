#include "utils.h"
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

std::string piece_type_to_string(piece::Type type) {
    switch (type) {
    case piece::Type::PAWN:
        return "Pawn";
    case piece::Type::KNIGHT:
        return "Knight";
    case piece::Type::BISHOP:
        return "Bishop";
    case piece::Type::ROOK:
        return "Rook";
    case piece::Type::QUEEN:
        return "Queen";
    case piece::Type::KING:
        return "King";
    default:
        return "Unknown";
    }
}

std::string move_type_to_string(moves::Type type) {
    switch (type) {
    case moves::Type::NORMAL:
        return "Normal";
    case moves::Type::CAPTURE:
        return "Capture";
    case moves::Type::CASTLING:
        return "Castling";
    case moves::Type::EN_PASSANT:
        return "En-Passant";
    case moves::Type::PROMOTION:
        return "Promotion";
    default:
        return "Unknown";
    }
}

std::string piece_color_to_string(piece::Color color) {
    return (color == piece::Color::WHITE) ? "White" : "Black";
}

} // namespace utils