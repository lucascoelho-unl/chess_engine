#ifndef CHESS_ENGINE_MOVES_H
#define CHESS_ENGINE_MOVES_H

#include "bitboard.h"
#include "board.h"
#include "square.h"
#include <vector>

namespace chess_engine {
namespace moves {

struct Move {
    square::Square from;
    square::Square to;
};

// Get moves from a specific square for a piece of the specified color
bit::Bitboard get_pawn_moves(square::Square from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_knight_moves(square::Square from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_bishop_moves(square::Square from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_rook_moves(square::Square from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_queen_moves(square::Square from, board::piece::Color color, const board::Board &board);
bit::Bitboard get_king_moves(square::Square from, board::piece::Color color, const board::Board &board);

// Get all possible moves for the specified color
bit::Bitboard get_all_pawn_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_knight_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_bishop_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_rook_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_queen_moves(board::piece::Color color, const board::Board &board);
bit::Bitboard get_all_king_moves(board::piece::Color color, const board::Board &board);

// General function to get moves for any piece type
bit::Bitboard get_piece_moves(board::piece::Type piece, square::Square from, board::piece::Color color, bit::Bitboard occupancy);

// Generate all valid moves for the specified color
std::vector<Move> generate_all_valid_moves(board::piece::Color color);

} // namespace moves
} // namespace chess_engine

#endif