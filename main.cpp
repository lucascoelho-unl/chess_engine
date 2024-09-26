#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>

using namespace chess_engine;

int main() {
    board::Board board = board::set_position("Q7/8/8/8/pppppppp/8/8/8");

    bit::Bitboard bishop_moves = moves::get_queen_moves(square::A8, board::piece::Color::WHITE, board);

    std::cout << board.to_string() << std::endl;
    utils::print_bitboard(bishop_moves);

    return 0;
}
