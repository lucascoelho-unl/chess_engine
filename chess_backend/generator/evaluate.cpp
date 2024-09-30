#include "../enums.h"
#include "../structure/board.h"
#include "../structure/game_state.h"

namespace chess_engine {
namespace evaluate {

int evaluate(game_state::Game_State &state) {
    board::Board board = state.get_board();

    int white_score = 0;
    int black_score = 0;

    white_score += __builtin_popcountll(board.get_pawns(piece::WHITE)) * 100;
    white_score += __builtin_popcountll(board.get_knights(piece::WHITE)) * 320;
    white_score += __builtin_popcountll(board.get_bishops(piece::WHITE)) * 330;
    white_score += __builtin_popcountll(board.get_rooks(piece::WHITE)) * 500;
    white_score += __builtin_popcountll(board.get_queens(piece::WHITE)) * 900;

    black_score += __builtin_popcountll(board.get_pawns(piece::BLACK)) * 100;
    black_score += __builtin_popcountll(board.get_knights(piece::BLACK)) * 320;
    black_score += __builtin_popcountll(board.get_bishops(piece::BLACK)) * 330;
    black_score += __builtin_popcountll(board.get_rooks(piece::BLACK)) * 500;
    black_score += __builtin_popcountll(board.get_queens(piece::BLACK)) * 900;

    return white_score - black_score; // Positive if white is winning, negative if black is winning
}

} // namespace evaluate
} // namespace chess_engine