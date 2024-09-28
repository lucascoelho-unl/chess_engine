// move_type.h
#ifndef CHESS_ENGINE_MOVE_TYPE_H
#define CHESS_ENGINE_MOVE_TYPE_H

namespace chess_engine {
namespace moves {

enum Move_Type {
    NORMAL = 0,
    CAPTURE = 1,
    EN_PASSANT = 2,
    CASTLING = 3,
    PROMOTION = 4,
};

} // namespace moves
} // namespace chess_engine

#endif
