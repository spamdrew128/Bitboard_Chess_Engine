#include <stdio.h>
#include <string.h>

#include "bench.h"
#include "timer.h"
#include "perft_table_entries.h"
#include "chess_search.h"
#include "board_info.h"
#include "game_state.h"
#include "zobrist.h"
#include "FEN.h"

bool Bench(int argc, char** argv) {
    if(argc != 2 || strcmp(argv[1], "bench")) {
        return true; // keep running
    }

    FEN_t fenList[] = { PERFT_TEST_TABLE(EXPAND_AS_FEN_ARRAY) };
    BoardInfo_t boardInfo;
    GameStack_t gameStack;
    ZobristStack_t zobristStack;

    Stopwatch_t stopwatch;
    StopwatchInit(&stopwatch);
    NodeCount_t nodeCount = 0;
    for(int i = 0; i < NUM_PERFT_ENTRIES; i++) {
        InterpretFEN(fenList[i], &boardInfo, &gameStack, &zobristStack);
        nodeCount += BenchSearch(&boardInfo, &gameStack, &zobristStack, 5);
    }

    Milliseconds_t msec = ElapsedTime(&stopwatch);
    printf("%lld nodes %lld nps\n", (long long)nodeCount, (long long)(nodeCount * msec_per_sec) / msec);

    return false;
}