#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>

using namespace chess_engine;

int main() {
    // Test 1: Rook in the middle of the board with no blockers
    // board::Board board = board::set_position("1R6/p1pppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // bit::Bitboard rook_moves = moves::get_rook_moves(square::B8, board::piece::Color::WHITE, board1);
    // std::cout << board.to_string() << std::endl;
    // utils::print_bitboard(rook_moves);

    board::Board board = board::set_position("8/8/3B4/4B3/5b2/8/8/8");

    bit::Bitboard bishop_moves = moves::get_bishop_moves(square::E5, board::piece::Color::WHITE, board);

    utils::print_bitboard(bishop_moves);

    return 0;
}
