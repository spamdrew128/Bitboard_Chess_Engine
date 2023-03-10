#ifndef __ENDINGS_H__
#define __ENDINGS_H__

#include <stdint.h>

#include "board_constants.h"
#include "board_info.h"
#include "game_state.h"
#include "movegen.h"
#include "zobrist.h"

typedef uint8_t GameEndStatus_t;
enum {
    ongoing,
    checkmate,
    draw
};

GameEndStatus_t CheckForMates(BoardInfo_t* boardInfo, int moveListMaxIndex);

GameEndStatus_t CurrentGameEndStatus(
    BoardInfo_t* boardInfo,
    GameStack_t* gameStack,
    ZobristStack_t* zobristStack,
    int moveListMaxIndex
);

#endif