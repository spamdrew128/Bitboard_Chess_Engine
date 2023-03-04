#ifndef __TRANSPOSITION_TABLE_H__
#define __TRANSPOSITION_TABLE_H__

#include <stdint.h>
#include <stdbool.h>

#include "move.h"
#include "engine_types.h"
#include "zobrist.h"

typedef uint8_t TTFlag_t;
enum {
    uninitialized_flag,
    lower_bound,
    exact,
    upper_bound
};

typedef struct {
    TTFlag_t flag;
    Depth_t depth;
    Move_t move;
    EvalScore_t score;
    ZobristHash_t hash;
} TTEntry_t;

typedef uint64_t TTLength_t;

typedef struct {
    TTEntry_t* entries;
    TTLength_t numEntries;
} TranspositionTable_t;


void TranspositionTableInit(TranspositionTable_t* table, Megabytes_t megabytes);

void TeardownTT(TranspositionTable_t* table);

#endif