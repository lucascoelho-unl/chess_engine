#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include "bitboard.h"
#include <string>

namespace chess_engine {
namespace board {

using namespace chess_engine::bit;

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
    Bitboard wp, wb, wn, wr, wq, wk = 0ULL; // White pieces
    Bitboard bp, bb, bn, br, bq, bk = 0ULL; // Black pieces

  public:
    Board(Bitboard wp, Bitboard wb, Bitboard wn, Bitboard wr, Bitboard wq, Bitboard wk,
          Bitboard bp, Bitboard bb, Bitboard bn, Bitboard br, Bitboard bq, Bitboard bk)
        : wp(wp), wb(wb), wn(wn), wr(wr), wq(wq), wk(wk),
          bp(bp), bb(bb), bn(bn), br(br), bq(bq), bk(bk) {}

    inline Bitboard get_white_pieces() const {
        return (wp | wb | wn | wr | wq | wk);
    }

    inline Bitboard get_black_pieces() const {
        return (bp | bb | bn | br | bq | bk);
    }

    inline Bitboard get_occupied_squares() const {
        return (get_white_pieces() | get_black_pieces());
    }

    inline Bitboard get_empty_squares() const {
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
    inline const Bitboard &get_white_pawns() const {
        return wp;
    }

    inline const Bitboard &get_white_knights() const {
        return wn;
    }

    inline const Bitboard &get_white_bishops() const {
        return wb;
    }

    inline const Bitboard &get_white_rooks() const {
        return wr;
    }

    inline const Bitboard &get_white_queen() const {
        return wq;
    }

    inline const Bitboard &get_white_king() const {
        return wk;
    }

    // Getters for black pieces
    inline const Bitboard &get_black_pawns() const {
        return bp;
    }

    inline const Bitboard &get_black_knights() const {
        return bn;
    }

    inline const Bitboard &get_black_bishops() const {
        return bb;
    }

    inline const Bitboard &get_black_rooks() const {
        return br;
    }

    inline const Bitboard &get_black_queen() const {
        return bq;
    }

    inline const Bitboard &get_black_king() const {
        return bk;
    }
};

} // namespace board
} // namespace chess_engine
#endif