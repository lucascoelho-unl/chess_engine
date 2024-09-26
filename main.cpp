#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>

using namespace chess_engine;

int main() {

    board::Board board = board::set_position("1b1r1b1r/8/8/8/8/8/8/8");

    bit::Bitboard all_moves = moves::generate_all_piece_moves(board::piece::Color::BLACK, board);

    std::cout << board.to_string() << std::endl;
    utils::print_bitboard(all_moves);

    return 0;
}
