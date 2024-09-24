#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>

#include "pieces/knight.h"

using namespace chess_engine;

int main() {
    board::Board board = board::set_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    std::cout << board.to_string() << std::endl;

    bit::Bitboard knight_moves = moves::get_knight_moves(square::B1, board::piece::WHITE, board);

    utils::print_bitboard(knight_moves);

    return 0;
}
