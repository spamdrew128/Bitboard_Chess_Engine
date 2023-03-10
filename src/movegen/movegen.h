#ifndef __MOVEGEN_H__
#define __MOVEGEN_H__

#include "board_constants.h"
#include "board_info.h"
#include "move.h"
#include "game_state.h"

enum {
    movelist_empty = -1
};

typedef struct {
    Move_t moves[MOVELIST_MAX];
    int maxCapturesIndex;
    int maxIndex;
} MoveList_t;

void CompleteMovegen(MoveList_t* moveList, BoardInfo_t* boardInfo, GameStack_t* stack);

bool EnPassantIsLegal(BoardInfo_t* boardInfo, Bitboard_t toBB, Bitboard_t fromBB, Color_t color);

#endif
