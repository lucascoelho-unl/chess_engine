#include "game_state.h"
#include "../enums.h"
#include "../moves/moves.h"
#include "../pieces/king.h"
#include <chrono>
#include <iostream>
#include <limits>
#include <sstream>
#include <thread>

namespace chess_engine {
namespace game_state {

GameState::GameState(const board::Board &board, piece::Color turn, bool w_k_castle, bool w_q_castle,
                     bool b_k_castle, bool b_q_castle, int en_passant, int halfmove, int fullmove)
    : board(board), turn(turn), white_castle_kingside(w_k_castle),
      white_castle_queenside(w_q_castle), black_castle_kingside(b_k_castle),
      black_castle_queenside(b_q_castle), en_passant_square(en_passant),
      halfmove_clock(halfmove), fullmove_number(fullmove) {}

const bool GameState::is_in_check(piece::Color color) const {
    bit::Bitboard opposite_team_attacks = moves::get_all_piece_moves(utils::opposite_color(color), board, *this);
    bit::Bitboard current_king_position = board.get_king(color);

    if (current_king_position & opposite_team_attacks) {
        return true;
    }
    return false;
}

bool GameState::is_checkmate() {
    // Check if the current player is in check
    if (!is_in_check(turn)) {
        return false;
    }

    // Generate all possible moves for the current player
    std::vector<moves::Move> possible_moves = moves::generate_legal_moves(turn, *this);

    // If there are no legal moves and the player is in check, it's checkmate
    return possible_moves.empty();
}

bool GameState::is_stalemate() {
    // Check if the current player is not in check
    if (is_in_check(turn)) {
        return false;
    }

    // Generate all possible moves for the current player
    std::vector<moves::Move> possible_moves = moves::generate_legal_moves(turn, *this);

    // If there are no legal moves and the player is not in check, it's stalemate
    return possible_moves.empty();
}

bool GameState::is_draw_by_fifty_move_rule() {
    // Check for 50-move rule (halfmove_clock is reset after captures and pawn moves)
    return halfmove_clock >= 100;
}

bool GameState::is_game_over() {
    if (is_checkmate()) {
        return true;
    }

    if (is_stalemate()) {
        return true;
    }

    if (is_draw_by_fifty_move_rule()) {
        return true;
    }

    // You can add other draw conditions here, such as:
    // - Threefold repetition
    // - Insufficient material

    return false;
}

bool GameState::is_square_attacked(int sq, piece::Color color) const {
    bit::Bitboard attacked_squares = 0ULL;

    // Generate moves for all pieces except the king                                                exclude king
    attacked_squares |= moves::get_all_piece_moves(utils::opposite_color(color), board, *this, true);

    // Check if the opponent's king attacks the square manually
    bit::Bitboard opponent_king = board.get_king(utils::opposite_color(color));
    int king_square = __builtin_ffsll(opponent_king) - 1;
    bit::Bitboard king_moves = king::king_moves[king_square];
    attacked_squares |= king_moves;

    return (((1ULL << sq) & attacked_squares) != 0);
}

void GameState::switch_turn() {
    turn = (turn == piece::WHITE) ? piece::BLACK : piece::WHITE;
}

void GameState::update_castling_rights(int from, int to) {
    // If the king moves, lose castling rights
    if (board.get_piece_type(from, turn) == piece::KING) {
        if (turn == piece::WHITE) {
            white_castle_kingside = white_castle_queenside = false;
        } else {
            black_castle_kingside = black_castle_queenside = false;
        }
    }
    // If a rook moves, update castling rights accordingly
    if (board.get_piece_type(from, turn) == piece::ROOK) {
        if (turn == piece::WHITE) {
            if (from == square::H1)
                white_castle_kingside = false;
            if (from == square::A1)
                white_castle_queenside = false;
        } else {
            if (from == square::H8)
                black_castle_kingside = false;
            if (from == square::A8)
                black_castle_queenside = false;
        }
    }
    // If a rook is captured, update castling rights
    if (board.get_piece_type(to, utils::opposite_color(turn)) == piece::Type::ROOK) {
        if (to == square::H1)
            white_castle_kingside = false;
        if (to == square::A1)
            white_castle_queenside = false;
        if (to == square::H8)
            black_castle_kingside = false;
        if (to == square::A8)
            black_castle_queenside = false;
    }
}

void GameState::update_en_passant(int from, int to) {
    en_passant_square = -1; // Reset en passant by default
    // If a pawn moves two squares, set up en passant square
    if (board.get_piece_type(from, turn) == piece::PAWN) {
        if (abs(from - to) == 16) {
            en_passant_square = (from + to) / 2;
        }
    }
}

bool GameState::is_castling_valid(int from, int to, piece::Color color) const {

    // Ensure that the move is either kingside or queenside castling
    if (!(to == square::G1 || to == square::C1 || to == square::G8 || to == square::C8)) {
        return false;
    }

    // Ensure the king has not moved before
    if (color == piece::WHITE && (from != square::E1 || !white_castle_kingside && !white_castle_queenside)) {
        return false; // Invalid white castling
    }
    if (color == piece::BLACK && (from != square::E8 || !black_castle_kingside && !black_castle_queenside)) {
        return false; // Invalid black castling
    }

    // Check specific castling conditions
    if (to == square::G1 && white_castle_kingside) {
        return !board.is_occupied(square::F1) && !board.is_occupied(square::G1) &&
               !is_square_attacked(square::E1, piece::WHITE) &&
               !is_square_attacked(square::F1, piece::WHITE) &&
               !is_square_attacked(square::G1, piece::WHITE) &&
               (get_board().get_piece_type(7, piece::Color::WHITE) == piece::Type::ROOK);
    } else if (to == square::C1 && white_castle_queenside) {
        return !board.is_occupied(square::B1) && !board.is_occupied(square::C1) && !board.is_occupied(square::D1) &&
               !is_square_attacked(square::E1, piece::WHITE) &&
               !is_square_attacked(square::D1, piece::WHITE) &&
               !is_square_attacked(square::C1, piece::WHITE) &&
               (get_board().get_piece_type(0, piece::Color::WHITE) == piece::Type::ROOK);
    } else if (to == square::G8 && black_castle_kingside) {
        return !board.is_occupied(square::F8) && !board.is_occupied(square::G8) &&
               !is_square_attacked(square::E8, piece::BLACK) &&
               !is_square_attacked(square::F8, piece::BLACK) &&
               !is_square_attacked(square::G8, piece::BLACK) &&
               (get_board().get_piece_type(63, piece::Color::BLACK) == piece::Type::ROOK);
    } else if (to == square::C8 && black_castle_queenside) {
        return !board.is_occupied(square::B8) && !board.is_occupied(square::C8) && !board.is_occupied(square::D8) &&
               !is_square_attacked(square::E8, piece::BLACK) &&
               !is_square_attacked(square::D8, piece::BLACK) &&
               !is_square_attacked(square::C8, piece::BLACK) &&
               (get_board().get_piece_type(56, piece::Color::BLACK) == piece::Type::ROOK);
    }

    return false; // Invalid castling move
}

GameState GameState::copy() const {
    return GameState(this->board.copy(),
                     this->turn,
                     this->white_castle_kingside,
                     this->white_castle_queenside,
                     this->black_castle_kingside,
                     this->black_castle_queenside,
                     this->en_passant_square,
                     this->halfmove_clock,
                     this->fullmove_number);
}

bool GameState::make_move(moves::Move move) {
    int from = move.from;
    int to = move.to;
    piece::Type piece_type = move.piece_type;
    piece::Color color = move.color;
    moves::Type move_type = move.move_type;
    piece::Type promotion = move.promotion;

    // Prepare the reversible move before making changes
    moves::Reversible_Move rev_move;
    rev_move.move = move;
    rev_move.turn = turn;
    rev_move.captured_piece = board.get_piece_type(move.to, utils::opposite_color(move.color));
    rev_move.white_castle_kingside = white_castle_kingside;
    rev_move.white_castle_queenside = white_castle_queenside;
    rev_move.black_castle_kingside = black_castle_kingside;
    rev_move.black_castle_queenside = black_castle_queenside;
    rev_move.en_passant_square = en_passant_square;
    rev_move.halfmove_clock = halfmove_clock;

    // Get the bitboard for the moving piece
    bit::Bitboard &piece_bitboard = board.get_pieces(piece_type, turn);
    bit::Bitboard from_mask = 1ULL << from;
    bit::Bitboard to_mask = 1ULL << to;
    bit::Bitboard valid_moves = moves::get_piece_moves(from, piece_type, color, board, *this);

    if (!(valid_moves & to_mask)) {
        std::cout << "Invalid move! Not in piece valid moves generated." << std::endl;
        move_history.push_back(rev_move);
        return false;
    }

    // Wrong color moving
    if (color != turn) {
        std::cout << "Invalid move! Not this color's turn." << std::endl;
        move_history.push_back(rev_move);
        return false;
    }

    // Check if there's actually a piece of the given type on the 'from' square
    if (!(piece_bitboard & from_mask)) {
        std::cout << "Invalid move! No piece of type on square." << std::endl;
        move_history.push_back(rev_move);
        return false; // Invalid move, no piece to move
    }

    if (piece_type == piece::Type::KING) {
        // Ensure the king does not move more than one square in any direction
        if (move_type == moves::NORMAL) {
            int from_row = from / 8;
            int from_col = from % 8;
            int to_row = to / 8;
            int to_col = to % 8;

            int row_diff = std::abs(to_row - from_row);
            int col_diff = std::abs(to_col - from_col);

            // A king can move only one square horizontally, vertically, or diagonally
            if (row_diff > 1 || col_diff > 1) {
                std::cout << "Invalid move! King can't move more than one square." << std::endl;
                return false;
            }
        }
    }

    // Handle castling (make sure castling is valid)
    if (move_type == moves::Type::CASTLING) {
        if (!is_castling_valid(from, to, turn)) {
            std::cout << "Castling move is invalid!" << std::endl;
            unmake_move();
            return false;
        }

        // Kingside castling
        if (to == square::G1 || to == square::G8) {
            board.move_piece(from, to, piece_type, turn); // Move the king
            board.move_piece((turn == piece::Color::WHITE) ? square::H1 : square::H8,
                             (turn == piece::Color::WHITE) ? square::F1 : square::F8,
                             piece::Type::ROOK, turn); // Move the rook
        }
        // Queenside castling
        else if (to == square::C1 || to == square::C8) {
            board.move_piece(from, to, piece_type, turn); // Move the king
            board.move_piece((turn == piece::Color::WHITE) ? square::A1 : square::A8,
                             (turn == piece::Color::WHITE) ? square::D1 : square::D8,
                             piece::Type::ROOK, turn); // Move the rook
        }
    }

    // Handle en passant
    if (move_type == moves::Type::EN_PASSANT) {
        // Ensure the piece is a pawn and positioned correctly
        if (piece_type == piece::PAWN) {
            if (to == en_passant_square) {
                int captured_pawn_square = (turn == piece::Color::WHITE) ? to - 8 : to + 8;
                board.move_piece(from, to, piece_type, turn);
                board.remove_piece(captured_pawn_square, utils::opposite_color(turn));
            }
        }
    }

    // Handle promotion
    if (move_type == moves::Type::PROMOTION) {
        if (board.is_capture(to, turn)) {
            board.remove_piece(to, utils::opposite_color(turn));
        }
        board.move_piece(from, to, piece_type, turn);
        bit::Bitboard &new_piece_bitboard = board.get_pieces(promotion, turn);
        piece_bitboard &= ~to_mask;    // Remove the pawn
        new_piece_bitboard |= to_mask; // Add the promoted piece
    }

    // Handle captures
    if (move_type == moves::Type::CAPTURE) {
        if (!board.is_capture(to, turn)) {
            std::cout << "Invalid move! Not a capture." << std::endl;
            unmake_move();
            return false;
        }
        board.remove_piece(to, utils::opposite_color(turn));
        board.move_piece(from, to, piece_type, turn);
    }

    // Handle normal moves
    if (move_type == moves::Type::NORMAL) {
        board.move_piece(from, to, piece_type, turn); // Move the piece
    }

    // Check if the king is left in check
    if (is_in_check(color)) {
        std::cout << "Move leaves the king in check! Undoing move." << std::endl;
        std::cout << board.to_string() << std::endl;
        bit::Bitboard ml = moves::get_all_piece_moves(utils::opposite_color(color), board, *this);
        utils::print_bitboard(board.get_queens(color));
        utils::print_bitboard(ml);
        move_history.push_back(rev_move);
        // unmake_move();
        return false;
    }

    if (move_type == moves::Type::CAPTURE || piece_type == piece::Type::PAWN) {
        halfmove_clock = 0;
    } else {
        ++halfmove_clock;
    }

    ++fullmove_number;

    // Update castling rights and en passant
    update_castling_rights(from, to);
    update_en_passant(from, to);

    // Switch the turn (White to Black, or Black to White)
    switch_turn();

    // Push the reversible move to history **after** making the move
    move_history.push_back(rev_move);

    return true;
}

bool GameState::unmake_move() {
    if (move_history.empty())
        return false;

    moves::Reversible_Move rev_move = move_history.back();
    move_history.pop_back();

    // Undo the move on the board
    board.move_piece(rev_move.move.to, rev_move.move.from, rev_move.move.piece_type, rev_move.move.color);

    // Handle castling separately
    if (rev_move.move.move_type == moves::CASTLING) {
        if (rev_move.move.to == square::G1) {                                    // White kingside
            board.move_piece(square::F1, square::H1, piece::ROOK, piece::WHITE); // Move rook back
        } else if (rev_move.move.to == square::C1) {                             // White queenside
            board.move_piece(square::D1, square::A1, piece::ROOK, piece::WHITE); // Move rook back
        } else if (rev_move.move.to == square::G8) {                             // Black kingside
            board.move_piece(square::F8, square::H8, piece::ROOK, piece::BLACK); // Move rook back
        } else if (rev_move.move.to == square::C8) {                             // Black queenside
            board.move_piece(square::D8, square::A8, piece::ROOK, piece::BLACK); // Move rook back
        }
    }

    // Handle promotion reversal
    if (rev_move.move.move_type == moves::PROMOTION) {
        // Remove the promoted piece from the target square
        board.remove_piece(rev_move.move.to, rev_move.move.color);
        // Put the pawn back on the from square
        board.add_piece(rev_move.move.from, piece::PAWN, rev_move.move.color);
    }

    // Restore the captured piece, if any
    if (rev_move.captured_piece != piece::Type::EMPTY) {
        board.add_piece(rev_move.move.to, rev_move.captured_piece, utils::opposite_color(rev_move.move.color));
    }

    // Restore castling rights, en passant square, halfmove clock, etc.
    turn = rev_move.turn;
    white_castle_kingside = rev_move.white_castle_kingside;
    white_castle_queenside = rev_move.white_castle_queenside;
    black_castle_kingside = rev_move.black_castle_kingside;
    black_castle_queenside = rev_move.black_castle_queenside;
    en_passant_square = rev_move.en_passant_square;
    halfmove_clock = rev_move.halfmove_clock;
    fullmove_number = rev_move.fullmove_number;

    return true;
}

GameState set_game_state(const std::string &fen) {
    std::istringstream fen_stream(fen);
    std::string piece_placement, active_color_str, castling_rights, en_passant_target;
    int halfmove_clock, fullmove_number;

    // Parse FEN string
    fen_stream >> piece_placement >> active_color_str >> castling_rights >> en_passant_target >> halfmove_clock >> fullmove_number;

    // Set up the board
    board::Board board = board::set_position(piece_placement);

    // Determine active color
    piece::Color turn = (active_color_str == "w") ? piece::WHITE : piece::BLACK;

    // Parse castling rights
    bool white_castle_kingside = castling_rights.find('K') != std::string::npos;
    bool white_castle_queenside = castling_rights.find('Q') != std::string::npos;
    bool black_castle_kingside = castling_rights.find('k') != std::string::npos;
    bool black_castle_queenside = castling_rights.find('q') != std::string::npos;

    // Parse en passant target square
    int en_passant_square = -1; // Default is no en passant available
    if (en_passant_target != "-") {
        en_passant_square = square::string_to_square(en_passant_target);
    }

    // Create and return the GameState object
    return GameState(board, turn, white_castle_kingside, white_castle_queenside,
                     black_castle_kingside, black_castle_queenside, en_passant_square,
                     halfmove_clock, fullmove_number);
}

} // namespace game_state
} // namespace chess_engine