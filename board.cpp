#include "board.h"
#include "bitboard.h"
#include <iostream>

namespace chess_engine {
namespace board {

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