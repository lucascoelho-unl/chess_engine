#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include <string>

const int board_size = 64;

enum piece_type {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum piece_color {
    WHITE,
    BLACK
};

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
        std::string chessBoard[8][8];

        for (int i = 0; i < 64; ++i) {
            chessBoard[i / 8][i % 8] = " ";
        }

        for (int i = 0; i < 64; ++i) {
            if (((wp >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "P";
            if (((wb >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "B";
            if (((wn >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "N";
            if (((wr >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "R";
            if (((wq >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "Q";
            if (((wk >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "K";

            if (((bp >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "p";
            if (((bb >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "b";
            if (((bn >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "n";
            if (((br >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "r";
            if (((bq >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "q";
            if (((bk >> i) & 1) == 1)
                chessBoard[i / 8][i % 8] = "k";
        }

        std::string result = "";
        for (int i = 7; i >= 0; --i) {
            result += "[ ";
            for (int j = 0; j < 8; ++j) {
                result += chessBoard[i][j];
                if (j < 7) {
                    result += " ";
                }
            }
            result += " ]\n";
        }

        return result;
    }

    // Example setter for testing
    void create_white_pawn(std::string s, std::string e) {
        wp = set_bit(wp, e);
        wp = clear_bit(wp, s);
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

#endif
