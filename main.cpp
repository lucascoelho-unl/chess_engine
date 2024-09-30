#include "enums.h"
#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/game_state.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <vector>
// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"

#include "pawn.h"

using namespace chess_engine;

int main() {
    // List of FEN strings to test different board positions
    std::vector<std::string> fen_positions = {
        "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 0", // Starting position
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0"
        "8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50", // Mid-game example
        // Add more FEN strings as needed for testing
    };

    for (const std::string &fen : fen_positions) {
        // Create game state from FEN string
        game_state::Game_State game_state = game_state::set_game_state(fen);
        board::Board b = game_state.get_board();

        std::cout << b.to_string() << std::endl;

        // Extract all possible moves from this position
        std::vector<moves::Move> move_list = moves::extract_all_possible_moves(piece::Color::WHITE, b, game_state);

        // Print all the moves for the current position
        for (const moves::Move &move : move_list) {
            // Get the piece type, color, and move positions
            std::string piece_type = utils::piece_type_to_string(move.piece_type); // Assuming `piece_type` is a member
            std::string piece_color = utils::piece_color_to_string(move.color);    // Assuming `color` is a member
            std::string from_square = square::int_position_to_string(move.from);
            std::string to_square = square::int_position_to_string(move.to);

            // Print the information
            printf("%s %s: %s -> %s\n", piece_color.c_str(), piece_type.c_str(), from_square.c_str(), to_square.c_str());
        }

        std::cout << "Number of moves: " << move_list.size() << std::endl;
        std::cout << "--------------------" << std::endl;
    }

    return 0;
}

// std::cout << b.to_string() << std::endl;

// utils::print_bitboard(k);

// std::cout << "Initial position:" << std::endl;
// std::cout << game_state.get_board().to_string() << std::endl;

// // Simulating a white pawn move from e4 to e5 (normal move)
// square::Square from_square = square::E7;
// square::Square to_square = square::E5;

// // Convert input squares to indices
// int from = static_cast<int>(from_square);
// int to = static_cast<int>(to_square);

// // Determine the piece type
// piece::Type type = piece::Type::PAWN;

// // Determine color (White in this case)
// piece::Color color = piece::Color::BLACK;

// // Create the move
// moves::Move move(from, to, type, color);

// // Make the move
// game_state.make_move(from, to, type, color, moves::Type::NORMAL, piece::QUEEN);

// // Display updated board
// std::cout << "Board after move:" << std::endl;
// std::cout << game_state.get_board().to_string() << std::endl;
