#include "../enums.h"
#include "../structure/board.h"
#include "../structure/game_state.h"

namespace chess_engine {
namespace evaluate {

int evaluate(game_state::Game_State &state) {
    board::Board board = state.get_board();

    int white_score = 0;
    int black_score = 0;

    white_score += __builtin_popcountll(board.get_pawns(piece::WHITE)) * 10;
    white_score += __builtin_popcountll(board.get_knights(piece::WHITE)) * 32;
    white_score += __builtin_popcountll(board.get_bishops(piece::WHITE)) * 33;
    white_score += __builtin_popcountll(board.get_rooks(piece::WHITE)) * 50;
    white_score += __builtin_popcountll(board.get_queens(piece::WHITE)) * 90;

    black_score += __builtin_popcountll(board.get_pawns(piece::BLACK)) * 10;
    black_score += __builtin_popcountll(board.get_knights(piece::BLACK)) * 32;
    black_score += __builtin_popcountll(board.get_bishops(piece::BLACK)) * 33;
    black_score += __builtin_popcountll(board.get_rooks(piece::BLACK)) * 50;
    black_score += __builtin_popcountll(board.get_queens(piece::BLACK)) * 90;

    return white_score - black_score; // Positive if white is winning, negative if black is winning
}

int piece_value(piece::Type type) {
    switch (type) {
    case piece::Type::PAWN:
        return 10;
    case piece::Type::KNIGHT:
        return 32;
    case piece::Type::BISHOP:
        return 33;
    case piece::Type::ROOK:
        return 50;
    case piece::Type::QUEEN:
        return 90;
    }
    return 0;
}

} // namespace evaluate
} // namespace chess_engine