#include "moves/moves.h"
#include "structure/bitboard.h"
#include "structure/board.h"
#include "structure/square.h"
#include "utils.h"
#include <iostream>

using namespace chess_engine;

int main() {
    // Test 1: Rook in the middle of the board with no blockers
    std::cout << "Test 1: Rook in the middle of the board (E4) with no blockers\n";
    board::Board board1 = board::set_position("8/8/8/8/4R3/8/8/8");
    bit::Bitboard rook_moves1 = moves::get_rook_moves(square::E4, board::piece::Color::WHITE, board1);
    std::cout << board1.to_string() << std::endl;
    utils::print_bitboard(rook_moves1);

    // Test 2: Rook at the edge of the board (A1)
    std::cout << "\nTest 2: Rook at the edge of the board (A1)\n";
    board::Board board2 = board::set_position("R7/8/8/8/8/8/8/8");
    bit::Bitboard rook_moves2 = moves::get_rook_moves(square::A8, board::piece::Color::WHITE, board2);
    std::cout << board2.to_string() << std::endl;
    utils::print_bitboard(rook_moves2);

    // Test 3: Rook at the edge of the board (H8)
    std::cout << "\nTest 3: Rook at the edge of the board (H8)\n";
    board::Board board3 = board::set_position("8/8/8/8/8/8/8/7R");
    bit::Bitboard rook_moves3 = moves::get_rook_moves(square::H1, board::piece::Color::WHITE, board3);
    std::cout << board3.to_string() << std::endl;
    utils::print_bitboard(rook_moves3);

    // Test 4: Rook blocked by friendly pieces
    std::cout << "\nTest 4: Rook (E4) blocked by friendly pieces\n";
    board::Board board4 = board::set_position("8/8/3P1P2/3PR3/3P1P2/8/8/8");
    bit::Bitboard rook_moves4 = moves::get_rook_moves(square::E5, board::piece::Color::WHITE, board4);
    std::cout << board4.to_string() << std::endl;
    utils::print_bitboard(rook_moves4);

    // Test 5: Rook blocked by enemy pieces
    std::cout << "\nTest 5: Rook (E4) blocked by enemy pieces\n";
    board::Board board5 = board::set_position("8/8/3p1p2/3Pr3/3p1p2/8/8/8");
    bit::Bitboard rook_moves5 = moves::get_rook_moves(square::E5, board::piece::Color::BLACK, board5);
    std::cout << board5.to_string() << std::endl;
    utils::print_bitboard(rook_moves5);

    return 0;
}
