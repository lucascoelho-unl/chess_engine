#include "moves.h"
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
#include <condition_variable>
#include <deque>
#include <future>
#include <iostream>
#include <mutex>
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

// Function to generate all moves for pieces, using the thread pool
bit::Bitboard generate_all_piece_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state, bool exclude_king) {
    bit::Bitboard all_moves = 0ULL;

    // Generate moves for all pieces except the king if exclude_king is true
    for (auto type : {piece::Type::PAWN, piece::Type::KNIGHT, piece::Type::BISHOP, piece::Type::ROOK, piece::Type::QUEEN}) {
        all_moves |= get_all_moves_for_piece(type, color, board, game_state);
    }

    // Conditionally add king's moves
    if (!exclude_king) {
        all_moves |= get_all_moves_for_piece(piece::Type::KING, color, board, game_state);
    }

    return all_moves;
}

std::vector<Move> generate_moves_for_piece(int from, piece::Type type, piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    std::vector<Move> moves;

    bit::Bitboard attack_bitboard = get_piece_moves(from, type, color, board, game_state);
    bit::Bitboard opponent_occupancy = (color == piece::Color::WHITE) ? board.get_black_pieces() : board.get_white_pieces();

    while (attack_bitboard) {
        int to = __builtin_ffsll(attack_bitboard) - 1; // Find the first set bit
        attack_bitboard &= attack_bitboard - 1;        // Clear the least significant set bit

        moves::Type move_type = moves::NORMAL;
        piece::Type promotion = piece::Type::EMPTY;

        // Handle capture
        if (opponent_occupancy & (1ULL << to)) {
            move_type = moves::CAPTURE;
        }

        // Handle special pawn moves (promotion)
        if (type == piece::Type::PAWN) {
            if ((to <= 7 || to >= 56)) { // Pawn reaches promotion row
                move_type = moves::PROMOTION;
                for (auto promotion_type : {piece::Type::QUEEN, piece::Type::ROOK, piece::Type::BISHOP, piece::Type::KNIGHT}) {
                    moves.push_back(Move(from, to, type, color, move_type, promotion_type));
                }
                continue; // Skip normal move addition as promotions are handled
            }
            if (to == game_state.en_passant_square) {
                move_type = moves::EN_PASSANT;
            }
        }

        // Handle castling
        if (type == piece::Type::KING) {
            if ((color == piece::Color::WHITE && from == 4 && (to == 2 || to == 6)) ||
                (color == piece::Color::BLACK && from == 60 && (to == 56 || to == 63))) {
                move_type = moves::CASTLING;
            }
        }

        moves.push_back(Move(from, to, type, color, move_type, promotion));
    }

    return moves;
}

std::vector<Move> generate_pseudo_legal_moves(piece::Color color, const board::Board &board, const game_state::Game_State &game_state) {
    std::vector<std::future<std::vector<Move>>> futures;
    std::vector<Move> all_moves;
    all_moves.reserve(218);

    // Launch threads for different piece types
    for (auto type : {piece::Type::QUEEN, piece::Type::ROOK, piece::Type::KING, piece::Type::BISHOP, piece::Type::KNIGHT, piece::Type::PAWN}) {
        futures.push_back(std::async(std::launch::async, [&, type] {
            std::vector<Move> piece_moves;
            bit::Bitboard piece_bitboard = board.get_pieces(type, color);

            while (piece_bitboard) {
                int from_square = __builtin_ffsll(piece_bitboard) - 1;
                piece_bitboard &= piece_bitboard - 1;

                // Extract moves for this piece from this square
                auto moves_for_piece = generate_moves_for_piece(from_square, type, color, board, game_state);
                piece_moves.insert(piece_moves.end(), moves_for_piece.begin(), moves_for_piece.end());
            }
            return piece_moves;
        }));
    }

    // Collect results from all threads
    for (auto &f : futures) {
        auto piece_moves = f.get();
        all_moves.insert(all_moves.end(), piece_moves.begin(), piece_moves.end());
    }

    return all_moves;
}

std::vector<moves::Move> generate_legal_moves(piece::Color color, game_state::Game_State &game_state) {
    std::vector<moves::Move> pseudo_moves = generate_pseudo_legal_moves(color, game_state.get_board(), game_state);
    std::vector<moves::Move> legal_moves;

    for (const auto &move : pseudo_moves) {
        // If the piece is the king, check if it moves into an attacked square
        if (move.piece_type == piece::Type::KING) {
            if (game_state.is_square_attacked(move.to, color)) {
                continue; // Skip if king moves into check
            }
        }
        // Temporarily make the move and check if the king is left in check
        game_state.make_pseudo_move(move);
        if (!game_state.is_in_check(color)) {
            legal_moves.push_back(move);
        }
        game_state.unmake_move();
    }

    return legal_moves;
}

std::string to_string(const Move &move) {
    // Convert individual fields to string representations
    std::string from_str = square::int_position_to_string(move.from);
    std::string to_str = square::int_position_to_string(move.to);
    std::string piece_type_str = utils::piece_type_to_string(move.piece_type);
    std::string color_str = utils::piece_color_to_string(move.color);
    std::string move_type_str = utils::move_type_to_string(move.move_type);
    std::string promotion_str = (move.promotion != piece::Type::EMPTY) ? utils::piece_type_to_string(move.promotion) : "None";

    // Construct the final string with all details
    std::string result = "Move: " + color_str + " " + piece_type_str + " from " + from_str + " to " + to_str +
                         ", Move Type: " + move_type_str +
                         (move.promotion != piece::Type::EMPTY ? ", Promotion: " + promotion_str : "");

    return result;
}

} // namespace moves
} // namespace chess_engine
