#ifndef CHESS_ENGINE_MOVES_H
#define CHESS_ENGINE_MOVES_H

#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"
#include <vector>

namespace chess_engine {
namespace moves {

struct Move {
    int from;
    square::Square to;
};

extern bit::Bitboard corners_mask;

// Get moves from a specific square for a piece of the specified color
bit::Bitboard get_pawn_moves(int from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_knight_moves(int from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_bishop_moves(int from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_rook_moves(int from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_queen_moves(int from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_king_moves(int from, board::piece::Color color, const board::Board &board);

// Get all possible moves for the specified color
bit::Bitboard get_all_pawn_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_knight_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_bishop_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_rook_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_queen_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_king_moves(board::piece::Color color, const board::Board &board);

// General function to get moves for any piece type
bit::Bitboard get_piece_moves(int from, board::piece::Type piece, board::piece::Color color, const board::Board &board);

// Generate all valid moves for the specified color
bit::Bitboard generate_all_piece_moves(board::piece::Color color, const board::Board &board);

} // namespace moves
} // namespace chess_engine

#endif