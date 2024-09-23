#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include "bitboard.h"
#include <string>

namespace chess_engine {
namespace board {

const int board_size = 64;

namespace piece {
enum Type {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum Color {
    WHITE,
    BLACK
};
} // namespace piece

class Board {
  private:
    bit::Bitboard wp, wb, wn, wr, wq, wk = 0ULL; // White pieces
    bit::Bitboard bp, bb, bn, br, bq, bk = 0ULL; // Black pieces

  public:
    Board(bit::Bitboard wp, bit::Bitboard wb, bit::Bitboard wn, bit::Bitboard wr, bit::Bitboard wq, bit::Bitboard wk,
          bit::Bitboard bp, bit::Bitboard bb, bit::Bitboard bn, bit::Bitboard br, bit::Bitboard bq, bit::Bitboard bk)
        : wp(wp), wb(wb), wn(wn), wr(wr), wq(wq), wk(wk),
          bp(bp), bb(bb), bn(bn), br(br), bq(bq), bk(bk) {}

    inline bit::Bitboard get_white_pieces() const {
        return (wp | wb | wn | wr | wq | wk);
    }

    inline bit::Bitboard get_black_pieces() const {
        return (bp | bb | bn | br | bq | bk);
    }

    inline bit::Bitboard get_occupied_squares() const {
        return (get_white_pieces() | get_black_pieces());
    }

    inline bit::Bitboard get_empty_squares() const {
        return ~get_occupied_squares();
    }

    std::string to_string() const {
        std::string chessboard[8][8];

        for (int i = 0; i < 64; ++i) {
            chessboard[i / 8][i % 8] = " ";
        }

        for (int i = 0; i < 64; ++i) {
            if (((wp >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "P";
            if (((wb >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "B";
            if (((wn >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "N";
            if (((wr >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "R";
            if (((wq >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "Q";
            if (((wk >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "K";

            if (((bp >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "p";
            if (((bb >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "b";
            if (((bn >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "n";
            if (((br >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "r";
            if (((bq >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "q";
            if (((bk >> i) & 1) == 1)
                chessboard[i / 8][i % 8] = "k";
        }

        char label = 'h';
        std::string result = "    1 2 3 4 5 6 7 8\n";
        for (int i = 7; i >= 0; --i) {
            result += label;
            result += " [ ";
            for (int j = 0; j < 8; ++j) {
                result += chessboard[i][j];
                if (j < 7) {
                    result += " ";
                }
            }
            result += " ]\n";
            --label;
        }

        return result;
    }

    // Getters for white pieces
    inline const bit::Bitboard &get_white_pawns() const {
        return wp;
    }

    inline const bit::Bitboard &get_white_knights() const {
        return wn;
    }

    inline const bit::Bitboard &get_white_bishops() const {
        return wb;
    }

    inline const bit::Bitboard &get_white_rooks() const {
        return wr;
    }

    inline const bit::Bitboard &get_white_queen() const {
        return wq;
    }

    inline const bit::Bitboard &get_white_king() const {
        return wk;
    }

    // Getters for black pieces
    inline const bit::Bitboard &get_black_pawns() const {
        return bp;
    }

    inline const bit::Bitboard &get_black_knights() const {
        return bn;
    }

    inline const bit::Bitboard &get_black_bishops() const {
        return bb;
    }

    inline const bit::Bitboard &get_black_rooks() const {
        return br;
    }

    inline const bit::Bitboard &get_black_queen() const {
        return bq;
    }

    inline const bit::Bitboard &get_black_king() const {
        return bk;
    }
};

} // namespace board
} // namespace chess_engine
#endif