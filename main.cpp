#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>

using namespace chess_engine;

int main() {

    board::Board board = board::set_position("rnbqkbnr/pBppBppp/8/P7/8/8/8/3N3N");

    bit::Bitboard pawn_move = moves::get_all_bishop_moves(board::piece::Color::WHITE, board);

    std::cout << board.to_string() << std::endl;
    utils::print_bitboard(pawn_move);

    return 0;
}
