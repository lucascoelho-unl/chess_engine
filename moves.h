#ifndef MOVES_H
#define MOVES_H

#include "bitboard.h"
#include "board.h"
#include "square.h"
#include <vector>

struct Move {
    Square from;
    Square to;
};

// Get moves from a specific square for a piece of the specified color
Bitboard get_pawn_moves(Square from, Color color, const Board &board);
Bitboard get_knight_moves(Square from, Color color, const Board &board);
Bitboard get_bishop_moves(Square from, Color color, const Board &board);
Bitboard get_rook_moves(Square from, Color color, const Board &board);
Bitboard get_queen_moves(Square from, Color color, const Board &board);
Bitboard get_king_moves(Square from, Color color, const Board &board);

// Get all possible moves for the specified color
Bitboard get_all_pawn_moves(Color color, const Board &board);
Bitboard get_all_knight_moves(Color color, const Board &board);
Bitboard get_all_bishop_moves(Color color, const Board &board);
Bitboard get_all_rook_moves(Color color, const Board &board);
Bitboard get_all_queen_moves(Color color, const Board &board);
Bitboard get_all_king_moves(Color color, const Board &board);

// General function to get moves for any piece type
Bitboard get_piece_moves(Piece_Type piece, Square from, Color color, Bitboard occupancy);

// Generate all valid moves for the specified color
std::vector<Move> generate_all_valid_moves(Color color);

#endif
