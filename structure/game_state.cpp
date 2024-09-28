#include "game_state.h"
#include "../moves/moves.h"
#include <sstream>

namespace chess_engine {
namespace game_state {

Game_State::Game_State(const board::Board &board, board::piece::Color turn, bool w_k_castle, bool w_q_castle,
                       bool b_k_castle, bool b_q_castle, int en_passant, int halfmove, int fullmove)
    : current_board(board), turn(turn), white_castle_kingside(w_k_castle),
      white_castle_queenside(w_q_castle), black_castle_kingside(b_k_castle),
      black_castle_queenside(b_q_castle), en_passant_square(en_passant),
      halfmove_clock(halfmove), fullmove_number(fullmove) {}

bool Game_State::is_square_attacked(int sq, board::piece::Color color) const {
    bit::Bitboard attacked_squares = 0ULL;
    if (color == board::piece::Color::WHITE)
        attacked_squares |= moves::generate_all_piece_moves(board::piece::Color::WHITE, current_board, *this);
    if (color == board::piece::Color::BLACK)
        attacked_squares |= moves::generate_all_piece_moves(board::piece::Color::BLACK, current_board, *this);
    return (((1ULL << sq) & attacked_squares) != 0);
}

void Game_State::save_state() {
    previous_states.push(*this);
}

void Game_State::restore_previous_state() {
    if (!previous_states.empty()) {
        *this = previous_states.top();
        previous_states.pop();
    }
}

void Game_State::switch_turn() {
    turn = (turn == board::piece::WHITE) ? board::piece::BLACK : board::piece::WHITE;
}

void Game_State::update_castling_rights(int from, int to) {
    // If the king moves, lose castling rights
    if (current_board.get_piece_type(from, turn) == board::piece::KING) {
        if (turn == board::piece::WHITE) {
            white_castle_kingside = white_castle_queenside = false;
        } else {
            black_castle_kingside = black_castle_queenside = false;
        }
    }
    // If a rook moves, update castling rights accordingly
    if (current_board.get_piece_type(from, turn) == board::piece::ROOK) {
        if (turn == board::piece::WHITE) {
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
    if (current_board.get_piece_type(to, utils::opposite_color(turn)) == board::piece::Type::ROOK) {
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

void Game_State::update_en_passant(int from, int to) {
    en_passant_square = -1; // Reset en passant by default
    // If a pawn moves two squares, set up en passant square
    if (current_board.get_piece_type(from, turn) == board::piece::PAWN) {
        if (abs(from - to) == 16) {
            en_passant_square = (from + to) / 2;
        }
    }
}

bool Game_State::is_castling_valid(int from, int to, board::piece::Color color) {
    // Ensure that the move is either kingside or queenside castling
    if (!(to == square::G1 || to == square::C1 || to == square::G8 || to == square::C8)) {
        return false;
    }

    // Ensure the king has not moved before
    if (color == board::piece::WHITE && (from != square::E1 || !white_castle_kingside && !white_castle_queenside)) {
        return false; // Invalid white castling
    }
    if (color == board::piece::BLACK && (from != square::E8 || !black_castle_kingside && !black_castle_queenside)) {
        return false; // Invalid black castling
    }

    // Check specific castling conditions
    if (to == square::G1 && white_castle_kingside) {
        return !current_board.is_occupied(square::F1) && !current_board.is_occupied(square::G1) &&
               !is_square_attacked(square::E1, board::piece::BLACK) &&
               !is_square_attacked(square::F1, board::piece::BLACK) &&
               !is_square_attacked(square::G1, board::piece::BLACK);
    } else if (to == square::C1 && white_castle_queenside) {
        return !current_board.is_occupied(square::B1) && !current_board.is_occupied(square::C1) && !current_board.is_occupied(square::D1) &&
               !is_square_attacked(square::E1, board::piece::BLACK) &&
               !is_square_attacked(square::D1, board::piece::BLACK) &&
               !is_square_attacked(square::C1, board::piece::BLACK);
    } else if (to == square::G8 && black_castle_kingside) {
        return !current_board.is_occupied(square::F8) && !current_board.is_occupied(square::G8) &&
               !is_square_attacked(square::E8, board::piece::WHITE) &&
               !is_square_attacked(square::F8, board::piece::WHITE) &&
               !is_square_attacked(square::G8, board::piece::WHITE);
    } else if (to == square::C8 && black_castle_queenside) {
        return !current_board.is_occupied(square::B8) && !current_board.is_occupied(square::C8) && !current_board.is_occupied(square::D8) &&
               !is_square_attacked(square::E8, board::piece::WHITE) &&
               !is_square_attacked(square::D8, board::piece::WHITE) &&
               !is_square_attacked(square::C8, board::piece::WHITE);
    }

    return false; // Invalid castling move
}

bool Game_State::make_move(int from, int to, board::piece::Type piece_type, board::piece::Color color, moves::Move_Type move_type, board::piece::Type promotion) {
    // Get the bitboard for the moving piece
    bit::Bitboard &piece_bitboard = current_board.get_pieces(piece_type, turn);
    bit::Bitboard from_mask = 1ULL << from;
    bit::Bitboard to_mask = 1ULL << to;

    // Wrong color moving
    if (color != turn) {
        std::cout << color << turn << std::endl;
        return false;
    }

    // Check if there's actually a piece of the given type on the 'from' square
    if (!(piece_bitboard & from_mask)) {
        return false; // Invalid move, no piece to move
    }

    // Handle castling
    if (move_type == 2) {
        // Validate castling
        if (!is_castling_valid(from, to, turn)) {
            std::cout << "Castling move is invalid!" << std::endl;
            return false;
        }

        // Kingside castling
        if (to == square::G1 || to == square::G8) {
            current_board.move_piece(from, to, piece_type, turn); // Move the king
            current_board.move_piece((turn == board::piece::Color::WHITE) ? square::H1 : square::H8,
                                     (turn == board::piece::Color::WHITE) ? square::F1 : square::F8,
                                     board::piece::Type::ROOK, turn); // Move the rook
        }
        // Queenside castling
        else if (to == square::C1 || to == square::C8) {
            current_board.move_piece(from, to, piece_type, turn); // Move the king
            current_board.move_piece((turn == board::piece::Color::WHITE) ? square::A1 : square::A8,
                                     (turn == board::piece::Color::WHITE) ? square::D1 : square::D8,
                                     board::piece::Type::ROOK, turn); // Move the rook
        }
    }

    // Handle en passant
    if (move_type == 1) {
        // Ensure the piece is a pawn and positioned correctly
        if (piece_type == board::piece::PAWN) {
            if (to == en_passant_square) {
                // Ensure the pawn is on the correct rank for en passant
                if ((turn == board::piece::Color::WHITE && from / 8 == 4) || // White pawns must be on rank 5
                    (turn == board::piece::Color::BLACK && from / 8 == 3)) { // Black pawns must be on rank 4
                    // Now check the capture direction
                    if (std::abs(from % 8 - to % 8) == 1) { // Ensure the pawn moves diagonally by 1 file
                        int captured_pawn_square = (turn == board::piece::Color::WHITE) ? to - 8 : to + 8;
                        current_board.move_piece(from, to, piece_type, turn);
                        current_board.remove_piece(captured_pawn_square,
                                                   (turn == board::piece::Color::WHITE) ? board::piece::Color::BLACK : board::piece::Color::WHITE);
                    }
                }
            }
        }
        return false;
    }

    // Check if the move is in the piece valid moves bitboard.
    // Only perform check here because castling and en-passant are not in the valid moves bitboard.
    bit::Bitboard valid_moves = moves::get_piece_moves(from, piece_type, turn, current_board, *this);
    if (!(to_mask & valid_moves))
        return false;

    // Handle promotion
    if (move_type == 3) {
        current_board.move_piece(from, to, piece_type, turn); // Move the pawn
        bit::Bitboard &new_piece_bitboard = current_board.get_pieces(promotion, turn);
        piece_bitboard &= ~to_mask;    // Remove the pawn
        new_piece_bitboard |= to_mask; // Add a queen at the 'to' square
    }

    // Handle captures
    if (move_type == 4) {
        if (!current_board.is_capture(to, turn))
            return false;
        current_board.remove_piece(to, (turn == board::piece::Color::WHITE) ? board::piece::Color::BLACK : board::piece::Color::WHITE);
        current_board.move_piece(from, to, piece_type, turn);
    }

    // Handle normal moves
    if (move_type == 0) {
        current_board.move_piece(from, to, piece_type, turn); // Move the piece
    }

    // Update castling rights based on rook/king moves
    update_castling_rights(from, to);

    // Update en passant square
    update_en_passant(from, to);

    // Switch the turn (White to Black, or Black to White)
    switch_turn();

    return true;
}

Game_State set_game_state(const std::string &fen) {
    std::istringstream fen_stream(fen);
    std::string piece_placement, active_color_str, castling_rights, en_passant_target;
    int halfmove_clock, fullmove_number;

    // Parse FEN string
    fen_stream >> piece_placement >> active_color_str >> castling_rights >> en_passant_target >> halfmove_clock >> fullmove_number;

    // Set up the board
    board::Board board = board::set_position(piece_placement);

    // Determine active color
    board::piece::Color turn = (active_color_str == "w") ? board::piece::WHITE : board::piece::BLACK;

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

    // Create and return the Game_State object
    return Game_State(board, turn, white_castle_kingside, white_castle_queenside,
                      black_castle_kingside, black_castle_queenside, en_passant_square,
                      halfmove_clock, fullmove_number);
}

} // namespace game_state
} // namespace chess_engine