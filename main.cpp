#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>

using namespace chess_engine;

int main() {

    board::Board board = board::set_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    bit::Bitboard queen_moves = moves::get_queen_moves(square::D1, board::piece::Color::WHITE, board);

    bit::Bitboard king_moves = moves::get_king_moves(square::E1, board::piece::Color::WHITE, board);

    std::cout << board.to_string() << std::endl;
    utils::print_bitboard(king_moves);

    return 0;
}
