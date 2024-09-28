#ifndef CHESS_ENGINE_MOVES_H
#define CHESS_ENGINE_MOVES_H

#include "../enums.h"
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

inline bit::Bitboard get_pawn_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return pawn::get_moves(from, color, board, game_state);
}

inline bit::Bitboard get_knight_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return knight::get_moves(from, color, board, game_state);
}

inline bit::Bitboard get_rook_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return rook::get_moves(from, color, board, game_state);
}

inline bit::Bitboard get_bishop_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return bishop::get_moves(from, color, board, game_state);
}

inline bit::Bitboard get_queen_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return queen::get_moves(from, color, board, game_state);
}

inline bit::Bitboard get_king_moves(int from, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return king::get_moves(from, color, board, game_state);
}

template <typename Get_Moves_Func>
bit::Bitboard get_all_moves(piece::Type type, piece::Color color, const board::Board &board, const game_state::Game_State &game_state, Get_Moves_Func get_moves) {
    bit::Bitboard all_moves = 0ULL;
    bit::Bitboard curr_pieces = board.get_pieces(type, color);

    while (curr_pieces) {
        int square = __builtin_ctzll(curr_pieces);                // Get index of least significant bit
        curr_pieces &= curr_pieces - 1;                           // Remove the LSB
        all_moves |= get_moves(square, color, board, game_state); // Combine all possible moves
    }

    return all_moves;
}

bit::Bitboard get_all_moves_for_piece(piece::Type type, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    switch (type) {
    case piece::Type::PAWN:
        return get_all_moves(type, color, board, game_state, get_pawn_moves);
    case piece::Type::KNIGHT:
        return get_all_moves(type, color, board, game_state, get_knight_moves);
    case piece::Type::BISHOP:
        return get_all_moves(type, color, board, game_state, get_bishop_moves);
    case piece::Type::ROOK:
        return get_all_moves(type, color, board, game_state, get_rook_moves);
    case piece::Type::QUEEN:
        return get_all_moves(type, color, board, game_state, get_queen_moves);
    case piece::Type::KING:
        return get_all_moves(type, color, board, game_state, get_king_moves);
    default:
        return 0ULL;
    }
}

inline bool is_piece_at_position(int from, bit::Bitboard piece_positions) {
    return (piece_positions & (1ULL << from)) != 0;
}

bit::Bitboard get_all_pawn_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return get_all_moves(piece::Type::PAWN, color, board, game_state, get_pawn_moves);
}

bit::Bitboard get_all_knight_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return get_all_moves(piece::Type::KNIGHT, color, board, game_state, get_knight_moves);
}

bit::Bitboard get_all_bishop_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return get_all_moves(piece::Type::BISHOP, color, board, game_state, get_bishop_moves);
}

bit::Bitboard get_all_rook_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return get_all_moves(piece::Type::ROOK, color, board, game_state, get_rook_moves);
}

bit::Bitboard get_all_queen_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return get_all_moves(piece::Type::QUEEN, color, board, game_state, get_queen_moves);
}

bit::Bitboard get_all_king_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    return get_all_moves(piece::Type::KING, color, board, game_state, get_king_moves);
}

bit::Bitboard get_piece_moves(int from, piece::Type type, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    bit::Bitboard moves = 0ULL;
    bit::Bitboard current_piece_positions = board.get_pieces(type, color);

    if (!is_piece_at_position(from, current_piece_positions)) {
        return 0ULL;
    }

    switch (type) {
    case piece::Type::PAWN:
        moves = get_pawn_moves(from, color, board, game_state);
        break;
    case piece::Type::KNIGHT:
        moves = get_knight_moves(from, color, board, game_state);
        break;
    case piece::Type::BISHOP:
        moves = get_bishop_moves(from, color, board, game_state);
        break;
    case piece::Type::ROOK:
        moves = get_rook_moves(from, color, board, game_state);
        break;
    case piece::Type::QUEEN:
        moves = get_queen_moves(from, color, board, game_state);
        break;
    case piece::Type::KING:
        moves = get_king_moves(from, color, board, game_state);
        break;
    }
    return moves;
}

bit::Bitboard generate_all_piece_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    std::vector<std::future<bit::Bitboard>> move_futures;

    move_futures.emplace_back(std::async(std::launch::async, [&board, color, &game_state]() {
        return get_all_moves_for_piece(piece::Type::PAWN, color, board, game_state);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color, &game_state]() {
        return get_all_moves_for_piece(piece::Type::KNIGHT, color, board, game_state);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color, &game_state]() {
        return get_all_moves_for_piece(piece::Type::BISHOP, color, board, game_state);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color, &game_state]() {
        return get_all_moves_for_piece(piece::Type::ROOK, color, board, game_state);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color, &game_state]() {
        return get_all_moves_for_piece(piece::Type::QUEEN, color, board, game_state);
    }));
    move_futures.emplace_back(std::async(std::launch::async, [&board, color, &game_state]() {
        return get_all_moves_for_piece(piece::Type::KING, color, board, game_state);
    }));

    bit::Bitboard all_moves = 0ULL;
    for (auto &future : move_futures) {
        all_moves |= future.get();
    }
    return all_moves;
}

} // namespace moves
} // namespace chess_engine

#endif // CHESS_ENGINE_MOVES_H
