#ifndef CHESS_ENGINE_ENUMS_H
#define CHESS_ENGINE_ENUMS_H

namespace chess_engine {

namespace piece {
enum Type {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    EMPTY = 6
};

enum Color {
    WHITE = 0,
    BLACK = 1,
    EMPTY_C = 2
};
} // namespace piece

namespace moves {
enum Type {
    NORMAL = 0,
    CAPTURE = 1,
    EN_PASSANT = 2,
    CASTLING = 3,
    PROMOTION = 4,
};
} // namespace moves

namespace square {
// clang-format off
enum Square {
    A1 = 0,  B1,  C1,  D1,  E1,  F1,  G1,  H1,
    A2,  B2,  C2,  D2,  E2,  F2,  G2,  H2,
    A3,  B3,  C3,  D3,  E3,  F3,  G3,  H3,
    A4,  B4,  C4,  D4,  E4,  F4,  G4,  H4,
    A5,  B5,  C5,  D5,  E5,  F5,  G5,  H5,
    A6,  B6,  C6,  D6,  E6,  F6,  G6,  H6,
    A7,  B7,  C7,  D7,  E7,  F7,  G7,  H7,
    A8,  B8,  C8,  D8,  E8,  F8,  G8,  H8,
};
// clang-format on
} // namespace square

namespace transposition {

enum class NodeType {
    EXACT,
    ALPHA,
    BETA
};

}

} // namespace chess_engine

#endif