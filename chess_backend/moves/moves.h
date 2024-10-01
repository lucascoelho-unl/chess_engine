#ifndef CHESS_ENGINE_MOVES_H
#define CHESS_ENGINE_MOVES_H

#include "../enums.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"
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
    piece::Type piece_type;
    piece::Color color;
    moves::Type move_type;
    piece::Type promotion;

    Move() : from(-1), to(-1), piece_type(piece::Type::EMPTY), color(piece::Color::WHITE), move_type(moves::Type::NORMAL), promotion(piece::Type::EMPTY) {}

    Move(int from, int to, piece::Type piece_type, piece::Color color, moves::Type move_type = moves::Type::NORMAL, piece::Type promotion = piece::Type::EMPTY)
        : from(from), to(to), piece_type(piece_type), color(color), move_type(move_type), promotion(promotion) {}

    // Define the less-than operator
    bool operator<(const Move &other) const {
        if (from != other.from)
            return from < other.from;
        if (to != other.to)
            return to < other.to;
        if (piece_type != other.piece_type)
            return piece_type < other.piece_type;
        if (color != other.color)
            return color < other.color;
        if (move_type != other.move_type)
            return move_type < other.move_type;
        return promotion < other.promotion;
    }
};

struct Reversible_Move {
    Move move;
    piece::Color turn;
    piece::Type captured_piece;
    bool white_castle_kingside;
    bool white_castle_queenside;
    bool black_castle_kingside;
    bool black_castle_queenside;
    int en_passant_square;
    int halfmove_clock;
    int fullmove_number;
};

// Get moves from a specific square for a piece of the specified color
bit::Bitboard get_pawn_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_knight_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_bishop_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_rook_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_queen_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_king_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// Get all possible moves for the specified color
bit::Bitboard get_all_pawn_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_knight_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_bishop_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_rook_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_queen_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state);
bit::Bitboard get_all_king_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// General function to get moves for any piece type
bit::Bitboard get_piece_moves(int from, piece::Type type, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// Generate all valid moves for the specified color
bit::Bitboard generate_all_piece_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state, bool exclude_king = false);

// Method of generating a list of valid moves for a piece given its move bitboard.
std::vector<Move> generate_moves_for_piece(int from, piece::Type type, piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// Extract all possible moves for all pieces and returns a list of them.
std::vector<Move> generate_pseudo_legal_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state);

// Get only legal moves, no moves that leave the king in check are left here.
std::vector<moves::Move> generate_legal_moves(piece::Color color, game_state::Game_State &game_state);

std::string to_string(const Move &move);

} // namespace moves
} // namespace chess_engine

#endif