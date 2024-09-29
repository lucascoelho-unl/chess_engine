#include "enums.h"
#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/game_state.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>
#include <string>

using namespace chess_engine;

int main() {
    std::string fen = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";

    // Create game state from FEN string
    game_state::Game_State game_state = game_state::set_game_state(fen);

    std::cout << "Initial position:" << std::endl;
    std::cout << game_state.get_board().to_string() << std::endl;

    // Simulating a white pawn move from e4 to e5 (normal move)
    square::Square from_square = square::E7;
    square::Square to_square = square::E5;

    // Convert input squares to indices
    int from = static_cast<int>(from_square);
    int to = static_cast<int>(to_square);

    // Determine the piece type
    piece::Type type = piece::Type::PAWN;

    // Determine color (White in this case)
    piece::Color color = piece::Color::BLACK;

    // Create the move
    moves::Move move(from, to, type, color);

    // Make the move
    game_state.make_move(from, to, type, color, moves::Type::NORMAL, piece::QUEEN);

    // Display updated board
    std::cout << "Board after move:" << std::endl;
    std::cout << game_state.get_board().to_string() << std::endl;

    return 0;
}
