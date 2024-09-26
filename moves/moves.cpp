#include "moves.h"
#include "../pieces/bishop.h"
#include "../pieces/king.h"
#include "../pieces/knight.h"
#include "../pieces/pawn.h"
#include "../pieces/queen.h"
#include "../pieces/rook.h"
#include "../structure/bitboard.h"
#include "../structure/board.h"
#include "../structure/square.h"
#include "../utils.h"
#include "slide/diagonal.h"
#include "slide/straight.h"
#include <sstream>
#include <stdexcept>
#include <string>

namespace chess_engine {
namespace moves {

bit::Bitboard corners_mask = 35604928818740736ULL;

bit::Bitboard get_pawn_moves(int from, board::piece::Color color, const board::Board &board) {
    return pawn::get_moves(from, color, board);
}

bit::Bitboard get_knight_moves(int from, board::piece::Color color, const board::Board &board) {
    return knight::get_moves(from, color, board);
}

bit::Bitboard get_rook_moves(int from, board::piece::Color color, const board::Board &board) {
    return rook::get_moves(from, color, board);
}

bit::Bitboard get_bishop_moves(int from, board::piece::Color color, const board::Board &board) {
    return bishop::get_moves(from, color, board);
}

bit::Bitboard get_queen_moves(int from, board::piece::Color color, const board::Board &board) {
    return queen::get_moves(from, color, board);
}

bit::Bitboard get_king_moves(int from, board::piece::Color color, const board::Board &board) {
    return king::get_moves(from, color, board);
}

bit::Bitboard get_all_pawn_moves(board::piece::Color color, const board::Board &board) {
    bit::Bitboard all_pawn_moves = 0ULL;
    bit::Bitboard current_pawn_positions = (color == board::piece::Color::WHITE) ? board.get_white_pawns() : board.get_black_pawns();
    for (int sq = 0; sq < 64; ++sq) {
        if ((1ULL << sq) & current_pawn_positions) {
            all_pawn_moves |= get_pawn_moves(sq, color, board);
        }
    }
    return all_pawn_moves;
}

bit::Bitboard get_all_knight_moves(board::piece::Color color, const board::Board &board) {
    bit::Bitboard all_knight_moves = 0ULL;
    bit::Bitboard current_knight_positions = (color == board::piece::Color::WHITE) ? board.get_white_knights() : board.get_black_knights();
    for (int sq = 0; sq < 64; ++sq) {
        if ((1ULL << sq) & current_knight_positions) {
            all_knight_moves |= get_knight_moves(sq, color, board);
        }
    }
    return all_knight_moves;
}

bit::Bitboard get_all_bishop_moves(board::piece::Color color, const board::Board &board) {
    bit::Bitboard all_bishop_moves = 0ULL;
    bit::Bitboard current_bishop_positions = (color == board::piece::Color::WHITE) ? board.get_white_bishops() : board.get_black_bishops();
    for (int sq = 0; sq < 64; ++sq) {
        if ((1ULL << sq) & current_bishop_positions) {
            all_bishop_moves |= get_bishop_moves(sq, color, board);
        }
    }
    return all_bishop_moves;
}

bit::Bitboard get_all_rook_moves(board::piece::Color color, const board::Board &board) {
    bit::Bitboard all_rook_moves = 0ULL;
    bit::Bitboard current_rook_positions = (color == board::piece::Color::WHITE) ? board.get_white_rooks() : board.get_black_rooks();
    for (int sq = 0; sq < 64; ++sq) {
        if ((1ULL << sq) & current_rook_positions) {
            all_rook_moves |= get_rook_moves(sq, color, board);
        }
    }
    return all_rook_moves;
}

bit::Bitboard get_all_queen_moves(board::piece::Color color, const board::Board &board) {
    bit::Bitboard all_queen_moves = 0ULL;
    bit::Bitboard current_queen_positions = (color == board::piece::Color::WHITE) ? board.get_white_queens() : board.get_black_queens();
    for (int sq = 0; sq < 64; ++sq) {
        if ((1ULL << sq) & current_queen_positions) {
            all_queen_moves |= get_queen_moves(sq, color, board);
        }
    }
    return all_queen_moves;
}

bit::Bitboard get_all_king_moves(board::piece::Color color, const board::Board &board) {
    bit::Bitboard all_king_moves = 0ULL;
    bit::Bitboard current_king_positions = (color == board::piece::Color::WHITE) ? board.get_white_king() : board.get_black_king();
    for (int sq = 0; sq < 64; ++sq) {
        if ((1ULL << sq) & current_king_positions) {
            all_king_moves |= get_king_moves(sq, color, board);
        }
    }
    return all_king_moves;
}

bit::Bitboard get_piece_moves(int from, board::piece::Type piece, board::piece::Color color, const board::Board &board) {
    bit::Bitboard moves = 0ULL;
    bit::Bitboard current_piece_positions;

    // Get the bitboard for the current piece type
    switch (piece) {
    case board::piece::Type::PAWN:
        current_piece_positions = (color == board::piece::Color::WHITE) ? board.get_white_pawns() : board.get_black_pawns();
        break;
    case board::piece::Type::KNIGHT:
        current_piece_positions = (color == board::piece::Color::WHITE) ? board.get_white_knights() : board.get_black_knights();
        break;
    case board::piece::Type::BISHOP:
        current_piece_positions = (color == board::piece::Color::WHITE) ? board.get_white_bishops() : board.get_black_bishops();
        break;
    case board::piece::Type::ROOK:
        current_piece_positions = (color == board::piece::Color::WHITE) ? board.get_white_rooks() : board.get_black_rooks();
        break;
    case board::piece::Type::QUEEN:
        current_piece_positions = (color == board::piece::Color::WHITE) ? board.get_white_queens() : board.get_black_queens();
        break;
    case board::piece::Type::KING:
        current_piece_positions = (color == board::piece::Color::WHITE) ? board.get_white_king() : board.get_black_king();
        break;
    }

    if ((current_piece_positions & (1ULL << from)) == 0) {
        std::ostringstream oss;
        oss << piece << " at position " << static_cast<int>(from) << " does not exist";
        throw std::invalid_argument(oss.str());
    }

    // Loop through all squares and apply the respective move function
    switch (piece) {
    case board::piece::Type::PAWN:
        moves |= get_pawn_moves(from, color, board);
        break;
    case board::piece::Type::KNIGHT:
        moves |= get_knight_moves(from, color, board);
        break;
    case board::piece::Type::BISHOP:
        moves |= get_bishop_moves(from, color, board);
        break;
    case board::piece::Type::ROOK:
        moves |= get_rook_moves(from, color, board);
        break;
    case board::piece::Type::QUEEN:
        moves |= get_queen_moves(from, color, board);
        break;
    case board::piece::Type::KING:
        moves |= get_king_moves(from, color, board);
        break;
    }
    return moves;
}

} // namespace moves
} // namespace chess_engine