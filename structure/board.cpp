#include "board.h"
#include "../enums.h"
#include "bitboard.h"
#include <iostream>

namespace chess_engine {
namespace board {

bit::Bitboard a_file = 72340172838076673ULL;
bit::Bitboard b_file = 144680345676153346ULL;
bit::Bitboard c_file = 289360691352306692ULL;
bit::Bitboard d_file = 578721382704613384ULL;
bit::Bitboard e_file = 1157442765409226768ULL;
bit::Bitboard f_file = 2314885530818453536ULL;
bit::Bitboard g_file = 4629771061636907072ULL;
bit::Bitboard h_file = 9259542123273814144ULL;

bit::Bitboard rank_1 = 255ULL;
bit::Bitboard rank_2 = 65280ULL;
bit::Bitboard rank_3 = 16711680ULL;
bit::Bitboard rank_4 = 4278190080ULL;
bit::Bitboard rank_5 = 1095216660480ULL;
bit::Bitboard rank_6 = 280375465082880ULL;
bit::Bitboard rank_7 = 71776119061217280ULL;
bit::Bitboard rank_8 = 18374686479671623680ULL;

std::vector<int> Board::get_squares_with_piece(piece::Type type, piece::Color color) const {
    std::vector<int> squares;
    bit::Bitboard piece_bitboard = get_pieces(type, color);

    bit::Bitboard temp_bitboard = piece_bitboard;
    while (temp_bitboard) {
        int square = __builtin_ffsll(temp_bitboard) - 1; // Find the first set bit (square index)
        squares.push_back(square);
        temp_bitboard &= temp_bitboard - 1; // Clear the least significant set bit
    }

    return squares;
}

board::Board board::Board::copy() const {
    return Board(this->wp, this->wb, this->wn, this->wr, this->wq, this->wk,
                 this->bp, this->bb, this->bn, this->br, this->bq, this->bk);
}

Board set_position(std::string fen) {
    bit::Bitboard wp = 0ULL, wb = 0ULL, wn = 0ULL, wr = 0ULL, wq = 0ULL, wk = 0ULL;
    bit::Bitboard bp = 0ULL, bb = 0ULL, bn = 0ULL, br = 0ULL, bq = 0ULL, bk = 0ULL;

    int rank = 56;
    int file = 0;
    for (char c : fen) {

        int square = rank + file;

        if (c == 'P')
            wp = bit::set_bit(wp, square);
        else if (c == 'B')
            wb = bit::set_bit(wb, square);
        else if (c == 'N')
            wn = bit::set_bit(wn, square);
        else if (c == 'R')
            wr = bit::set_bit(wr, square);
        else if (c == 'Q')
            wq = bit::set_bit(wq, square);
        else if (c == 'K')
            wk = bit::set_bit(wk, square);

        else if (c == 'p')
            bp = bit::set_bit(bp, square);
        else if (c == 'b')
            bb = bit::set_bit(bb, square);
        else if (c == 'n')
            bn = bit::set_bit(bn, square);
        else if (c == 'r')
            br = bit::set_bit(br, square);
        else if (c == 'q')
            bq = bit::set_bit(bq, square);
        else if (c == 'k')
            bk = bit::set_bit(bk, square);
        else
            file += (c - '1');

        if (c == '/') {
            rank -= 8;
            file = 0;
        } else {
            ++file;
        }
    }

    return Board(wp, wb, wn, wr, wq, wk, bp, bb, bn, br, bq, bk);
}
} // namespace board
} // namespace chess_engine