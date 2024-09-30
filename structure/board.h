#ifndef CHESS_ENGINE_BOARD_H
#define CHESS_ENGINE_BOARD_H

#include "../enums.h"
#include "bitboard.h"
#include <string>
#include <vector>

namespace chess_engine {
namespace board {

const int board_size = 64;

extern bit::Bitboard a_file;
extern bit::Bitboard b_file;
extern bit::Bitboard c_file;
extern bit::Bitboard d_file;
extern bit::Bitboard e_file;
extern bit::Bitboard f_file;
extern bit::Bitboard g_file;
extern bit::Bitboard h_file;

extern bit::Bitboard rank_1;
extern bit::Bitboard rank_2;
extern bit::Bitboard rank_3;
extern bit::Bitboard rank_4;
extern bit::Bitboard rank_5;
extern bit::Bitboard rank_6;
extern bit::Bitboard rank_7;
extern bit::Bitboard rank_8;

class Board {
  private:
    bit::Bitboard wp, wb, wn, wr, wq, wk = 0ULL; // White pieces
    bit::Bitboard bp, bb, bn, br, bq, bk = 0ULL; // Black pieces

  public:
    Board(bit::Bitboard wp, bit::Bitboard wb, bit::Bitboard wn, bit::Bitboard wr, bit::Bitboard wq, bit::Bitboard wk,
          bit::Bitboard bp, bit::Bitboard bb, bit::Bitboard bn, bit::Bitboard br, bit::Bitboard bq, bit::Bitboard bk)
        : wp(wp), wb(wb), wn(wn), wr(wr), wq(wq), wk(wk),
          bp(bp), bb(bb), bn(bn), br(br), bq(bq), bk(bk) {}

    bool operator==(const Board &other) const {
        return wp == other.wp &&
               wb == other.wb &&
               wn == other.wn &&
               wr == other.wr &&
               wq == other.wq &&
               wk == other.wk &&
               bp == other.bp &&
               bb == other.bb &&
               bn == other.bn &&
               br == other.br &&
               bq == other.bq &&
               bk == other.bk;
    }

    // Combined white or black pieces
    inline bit::Bitboard get_white_pieces() const {
        return (wp | wb | wn | wr | wq | wk);
    }

    inline bit::Bitboard get_black_pieces() const {
        return (bp | bb | bn | br | bq | bk);
    }

    // Occupied and empty squares
    inline bit::Bitboard get_occupied_squares() const {
        return (get_white_pieces() | get_black_pieces());
    }

    inline bit::Bitboard get_empty_squares() const {
        return ~get_occupied_squares();
    }

    inline bool is_occupied(int sq) const {
        return ((1ULL << sq) & get_occupied_squares());
    }

    inline bool is_occupied(square::Square square) const {
        int sq = static_cast<int>(square);
        return ((1ULL << sq) & get_occupied_squares());
    }

    // to_string() function for printing the board
    std::string to_string() const {
        std::string chessboard[8][8];

        for (int i = 0; i < 64; ++i) {
            chessboard[i / 8][i % 8] = " ";
        }

        // clang-format off
        for (int i = 0; i < 64; ++i) {
            if (((wp >> i) & 1) == 1) chessboard[i / 8][i % 8] = "P";
            if (((wb >> i) & 1) == 1) chessboard[i / 8][i % 8] = "B";
            if (((wn >> i) & 1) == 1) chessboard[i / 8][i % 8] = "N";
            if (((wr >> i) & 1) == 1) chessboard[i / 8][i % 8] = "R";
            if (((wq >> i) & 1) == 1) chessboard[i / 8][i % 8] = "Q";
            if (((wk >> i) & 1) == 1) chessboard[i / 8][i % 8] = "K";

            if (((bp >> i) & 1) == 1) chessboard[i / 8][i % 8] = "p";
            if (((bb >> i) & 1) == 1) chessboard[i / 8][i % 8] = "b";
            if (((bn >> i) & 1) == 1) chessboard[i / 8][i % 8] = "n";
            if (((br >> i) & 1) == 1) chessboard[i / 8][i % 8] = "r";
            if (((bq >> i) & 1) == 1) chessboard[i / 8][i % 8] = "q";
            if (((bk >> i) & 1) == 1) chessboard[i / 8][i % 8] = "k";
        }
        // clang-format on

        char label = '8';
        std::string result = "     a b c d e f g h\n\n";
        for (int i = 7; i >= 0; --i) {
            result += label;
            result += "  [ ";
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

    // Getters for pieces based on color
    inline const bit::Bitboard &get_pawns(piece::Color color) const {
        return (color == piece::Color::WHITE) ? wp : bp;
    }

    inline const bit::Bitboard &get_knights(piece::Color color) const {
        return (color == piece::Color::WHITE) ? wn : bn;
    }

    inline const bit::Bitboard &get_bishops(piece::Color color) const {
        return (color == piece::Color::WHITE) ? wb : bb;
    }

    inline const bit::Bitboard &get_rooks(piece::Color color) const {
        return (color == piece::Color::WHITE) ? wr : br;
    }

    inline const bit::Bitboard &get_queens(piece::Color color) const {
        return (color == piece::Color::WHITE) ? wq : bq;
    }

    inline const bit::Bitboard &get_king(piece::Color color) const {
        return (color == piece::Color::WHITE) ? wk : bk;
    }

    inline piece::Type get_piece_type(int sq, piece::Color color) const {
        bit::Bitboard mask = (1ULL << sq);

        if (mask & get_pawns(color))
            return piece::Type::PAWN;
        if (mask & get_bishops(color))
            return piece::Type::BISHOP;
        if (mask & get_knights(color))
            return piece::Type::KNIGHT;
        if (mask & get_rooks(color))
            return piece::Type::ROOK;
        if (mask & get_queens(color))
            return piece::Type::QUEEN;
        if (mask & get_king(color))
            return piece::Type::KING;
        return piece::Type::EMPTY;
    }

    inline bit::Bitboard &get_pieces(piece::Type type, piece::Color color) {
        switch (type) {
        case piece::PAWN:
            return (color == piece::Color::WHITE) ? wp : bp;
        case piece::KNIGHT:
            return (color == piece::Color::WHITE) ? wn : bn;
        case piece::BISHOP:
            return (color == piece::Color::WHITE) ? wb : bb;
        case piece::ROOK:
            return (color == piece::Color::WHITE) ? wr : br;
        case piece::QUEEN:
            return (color == piece::Color::WHITE) ? wq : bq;
        case piece::KING:
            return (color == piece::Color::WHITE) ? wk : bk;
        default:
            throw std::invalid_argument("Invalid piece type");
        }
    }

    // Const version for read-only access
    inline const bit::Bitboard &get_pieces(piece::Type type, piece::Color color) const {
        switch (type) {
        case piece::PAWN:
            return (color == piece::Color::WHITE) ? wp : bp;
        case piece::KNIGHT:
            return (color == piece::Color::WHITE) ? wn : bn;
        case piece::BISHOP:
            return (color == piece::Color::WHITE) ? wb : bb;
        case piece::ROOK:
            return (color == piece::Color::WHITE) ? wr : br;
        case piece::QUEEN:
            return (color == piece::Color::WHITE) ? wq : bq;
        case piece::KING:
            return (color == piece::Color::WHITE) ? wk : bk;
        default:
            throw std::invalid_argument("Invalid piece type");
        }
    }

    Board copy() const;

    bool is_capture(int to, piece::Color color) const {
        bit::Bitboard to_mask = 1ULL << to;
        if (color == piece::Color::WHITE) {
            return get_black_pieces() & to_mask; // Check if there is a black piece on the 'to' square
        } else {
            return get_white_pieces() & to_mask; // Check if there is a white piece on the 'to' square
        }
    }

    void remove_piece(int square, piece::Color color) {
        bit::Bitboard mask = ~(1ULL << square); // Mask to unset the piece at the given square
        // clang-format off
        if (color == piece::Color::WHITE) {
            if (wp & (1ULL << square)) wp &= mask;
            else if (wb & (1ULL << square)) wb &= mask;
            else if (wn & (1ULL << square)) wn &= mask;
            else if (wr & (1ULL << square)) wr &= mask;
            else if (wq & (1ULL << square)) wq &= mask;
            else if (wk & (1ULL << square)) wk &= mask;
        } else {
            if (bp & (1ULL << square)) bp &= mask;
            else if (bb & (1ULL << square)) bb &= mask;
            else if (bn & (1ULL << square)) bn &= mask;
            else if (br & (1ULL << square)) br &= mask;
            else if (bq & (1ULL << square)) bq &= mask;
            else if (bk & (1ULL << square)) bk &= mask;
        }
        // clang-format on
    }

    bool move_piece(int from, int to, piece::Type piece_type, piece::Color color) {
        bit::Bitboard &piece_bitboard = get_pieces(piece_type, color);
        bit::Bitboard from_mask = 1ULL << from;
        bit::Bitboard to_mask = 1ULL << to;

        // Remove the piece from the 'from' square
        piece_bitboard &= ~from_mask;

        // Add the piece to the 'to' square
        piece_bitboard |= to_mask;

        return true;
    }

    std::vector<int> get_squares_with_piece(piece::Type type, piece::Color color) const;
};

Board set_position(std::string fen);

} // namespace board
} // namespace chess_engine
#endif