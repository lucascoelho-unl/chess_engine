#ifndef CHESS_ENGINE_MOVES_H
#define CHESS_ENGINE_MOVES_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"
#include "move_type.h"
#include <vector>

// Forward declaration to avoid circular dependency
namespace chess_engine {
namespace game_state {

class Game_State; // Forward declare Game_State

} // namespace game_state

namespace moves {

struct Move {
    int from;
    int to;
    board::piece::Type piece_type;
    board::piece::Color color;
    moves::Move_Type move_type;
    board::piece::Type promotion;

    Move(int from, int to, board::piece::Type piece_type, board::piece::Color color, moves::Move_Type move_type = moves::Move_Type::NORMAL, board::piece::Type promotion = board::piece::Type::EMPTY)
        : from(from), to(to), piece_type(piece_type), color(color), move_type(move_type), promotion(promotion) {}
};

// Get moves from a specific square for a piece of the specified color
bit::Bitboard get_pawn_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_knight_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_bishop_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_rook_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_queen_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_king_moves(int from, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// Get all possible moves for the specified color
bit::Bitboard get_all_pawn_moves(board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_knight_moves(board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_bishop_moves(board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_rook_moves(board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_queen_moves(board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_king_moves(board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// General function to get moves for any piece type
bit::Bitboard get_piece_moves(int from, board::piece::Type piece, board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// Generate all valid moves for the specified color
bit::Bitboard generate_all_piece_moves(board::piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

} // namespace moves
} // namespace chess_engine

#endif