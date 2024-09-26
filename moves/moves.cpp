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
#include <future>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace chess_engine {
namespace moves {

inline bit::Bitboard get_pawn_moves(int from, board::piece::Color color, const board::Board &board) {
    return pawn::get_moves(from, color, board);
}

inline bit::Bitboard get_knight_moves(int from, board::piece::Color color, const board::Board &board) {
    return knight::get_moves(from, color, board);
}

inline bit::Bitboard get_rook_moves(int from, board::piece::Color color, const board::Board &board) {
    return rook::get_moves(from, color, board);
}

inline bit::Bitboard get_bishop_moves(int from, board::piece::Color color, const board::Board &board) {
    return bishop::get_moves(from, color, board);
}

inline bit::Bitboard get_queen_moves(int from, board::piece::Color color, const board::Board &board) {
    return queen::get_moves(from, color, board);
}

inline bit::Bitboard get_king_moves(int from, board::piece::Color color, const board::Board &board) {
    return king::get_moves(from, color, board);
}

template <typename Get_Moves_Func>
bit::Bitboard get_all_moves(board::piece::Type type, board::piece::Color color, const board::Board &board, Get_Moves_Func get_moves) {
    bit::Bitboard all_moves = 0ULL;
    bit::Bitboard curr_pieces = board.get_pieces(type, color);

    while (curr_pieces) {
        int square = __builtin_ctzll(curr_pieces);    // Get index of least significant bit
        curr_pieces &= curr_pieces - 1;               // Remove the LSB
        all_moves |= get_moves(square, color, board); // Combine all possible moves
    }

    return all_moves;
}

// Generalized function for all types
bit::Bitboard get_all_moves_for_piece(board::piece::Type type, board::piece::Color color, const board::Board &board) {
    switch (type) {
    case board::piece::Type::PAWN:
        return get_all_moves(type, color, board, get_pawn_moves);
    case board::piece::Type::KNIGHT:
        return get_all_moves(type, color, board, get_knight_moves);
    case board::piece::Type::BISHOP:
        return get_all_moves(type, color, board, get_bishop_moves);
    case board::piece::Type::ROOK:
        return get_all_moves(type, color, board, get_rook_moves);
    case board::piece::Type::QUEEN:
        return get_all_moves(type, color, board, get_queen_moves);
    case board::piece::Type::KING:
        return get_all_moves(type, color, board, get_king_moves);
    default:
        return 0ULL; // Return 0 if piece type is invalid
    }
}

inline bool is_piece_at_position(int from, bit::Bitboard piece_positions) {
    return (piece_positions & (1ULL << from)) != 0;
}

bit::Bitboard get_all_pawn_moves(board::piece::Color color, const board::Board &board) {
    return get_all_moves(board::piece::Type::PAWN, color, board, get_pawn_moves);
}

bit::Bitboard get_all_knight_moves(board::piece::Color color, const board::Board &board) {
    return get_all_moves(board::piece::Type::KNIGHT, color, board, get_knight_moves);
}

bit::Bitboard get_all_bishop_moves(board::piece::Color color, const board::Board &board) {
    return get_all_moves(board::piece::Type::BISHOP, color, board, get_bishop_moves);
}

bit::Bitboard get_all_rook_moves(board::piece::Color color, const board::Board &board) {
    return get_all_moves(board::piece::Type::ROOK, color, board, get_rook_moves);
}

bit::Bitboard get_all_queen_moves(board::piece::Color color, const board::Board &board) {
    return get_all_moves(board::piece::Type::QUEEN, color, board, get_queen_moves);
}

bit::Bitboard get_all_king_moves(board::piece::Color color, const board::Board &board) {
    return get_all_moves(board::piece::Type::KING, color, board, get_king_moves);
}

bit::Bitboard get_piece_moves(int from, board::piece::Type type, board::piece::Color color, const board::Board &board) {
    bit::Bitboard moves = 0ULL;
    bit::Bitboard current_piece_positions = board.get_pieces(type, color);

    if (!is_piece_at_position(from, current_piece_positions)) {
        return 0ULL; // No need to throw exceptions, return invalid moves
    }

    // Switch case for respective piece move functions
    switch (type) {
    case board::piece::Type::PAWN:
        moves = get_pawn_moves(from, color, board);
        break;
    case board::piece::Type::KNIGHT:
        moves = get_knight_moves(from, color, board);
        break;
    case board::piece::Type::BISHOP:
        moves = get_bishop_moves(from, color, board);
        break;
    case board::piece::Type::ROOK:
        moves = get_rook_moves(from, color, board);
        break;
    case board::piece::Type::QUEEN:
        moves = get_queen_moves(from, color, board);
        break;
    case board::piece::Type::KING:
        moves = get_king_moves(from, color, board);
        break;
    }
    return moves;
}

bit::Bitboard generate_all_piece_moves(board::piece::Color color, const board::Board &board) {
    // Create a vector of futures to hold the async results
    std::vector<std::future<bit::Bitboard>> move_futures;

    // Launch asynchronous tasks to generate moves for each piece type
    move_futures.emplace_back(std::async(std::launch::async, [&board, color]() {
        return get_all_moves_for_piece(board::piece::Type::PAWN, color, board);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color]() {
        return get_all_moves_for_piece(board::piece::Type::KNIGHT, color, board);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color]() {
        return get_all_moves_for_piece(board::piece::Type::BISHOP, color, board);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color]() {
        return get_all_moves_for_piece(board::piece::Type::ROOK, color, board);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color]() {
        return get_all_moves_for_piece(board::piece::Type::QUEEN, color, board);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color]() {
        return get_all_moves_for_piece(board::piece::Type::KING, color, board);
    }));

    // Initialize a bitboard to hold all combined moves
    bit::Bitboard all_moves = 0ULL;
    for (auto &future : move_futures) {
        all_moves |= future.get(); // Wait for the future to be ready and combine the result
    }
    return all_moves;
}

} // namespace moves
} // namespace chess_engine
