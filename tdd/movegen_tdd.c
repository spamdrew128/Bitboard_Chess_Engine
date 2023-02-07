#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "debug.h"
#include "movegen_tdd.h"
#include "pieces.h"
#include "board_constants.h"
#include "lookup.h"
#include "move.h"
#include "game_state.h"

// HELPERS
static GameStack_t stack;

static void TestSetup() {
    InitGameStack(&stack);
}

static int CountPieceMoves(Piece_t piece, MoveList_t moveList, BoardInfo_t* info) {
    int count = 0;
    for(int i = 0; i <= moveList.maxIndex; i++) {
        Square_t fromSquare = ReadFromSquare(moveList.moves[i]);
        if(PieceOnSquare(info, fromSquare) == piece) {
            count++;
        }
    }

    return count;
}

// q5bk/1P6/2P1Q3/3K2Rr/8/3N1B2/3n4/3r4
static void InitPinPositionInfo(BoardInfo_t* info) {
    InitTestInfo(info, {
        info->kings[white] = CreateBitboard(1, d5);
        info->pawns[white] = CreateBitboard(2, c6,b7);
        info->rooks[white] = CreateBitboard(1, g5);
        info->bishops[white] = CreateBitboard(1, f3);
        info->knights[white] = CreateBitboard(1, d3);
        info->queens[white] = CreateBitboard(1, e6);

        info->kings[black] = CreateBitboard(1, h8);
        info->knights[black] = CreateBitboard(1, d2);
        info->bishops[black] = CreateBitboard(1, g8);
        info->rooks[black] = CreateBitboard(2, d1,h5);
        info->queens[black] = CreateBitboard(1, a8);
    });

    GetEmptyNextGameState(&stack);
}

// 8/8/PpP1k3/8/4K3/pPp5/8/8
static void InitDoubleEnPassantPosition(BoardInfo_t* info) {
    InitTestInfo(info, {
        info->kings[white] = CreateBitboard(1, e4);
        info->pawns[white] = CreateBitboard(3, b3,a6,c6);

        info->kings[black] = CreateBitboard(1, e6);
        info->pawns[black] = CreateBitboard(3, b6,a3,c3);
    });

    GameState_t* state = GetEmptyNextGameState(&stack);
    state->enPassantSquares = CreateBitboard(2, b2,b7);
}

// 1b6/8/2pP4/4KPpr/8/8/8/k7
static void InitTrickyPinnedEnPassantPostitionInfo(BoardInfo_t* info) {
    InitTestInfo(info, {
        info->kings[white] = CreateBitboard(1, e5);
        info->pawns[white] = CreateBitboard(2, f5,d6);

        info->kings[black] = CreateBitboard(1, a1);
        info->pawns[black] = CreateBitboard(2, g5,c6);
        info->bishops[black] = CreateBitboard(1, b8);
        info->rooks[black] = CreateBitboard(1, h5);
    });

    GameState_t* state = GetEmptyNextGameState(&stack);
    state->enPassantSquares = CreateBitboard(2, g6,c7);
}

// TESTS
static void ShouldCorrectlyEvaluateCapturesInPosWithPins() {
    TestSetup();
    BoardInfo_t info;
    InitPinPositionInfo(&info);

    int expectedNumKingCaptures = 0;
    int expectedNumPawnCaptures = 4;
    int expectedNumRookCaptures = 1;
    int expectedNumBishopCaptures = 2;
    int expectedNumKnightsCaptures = 0;
    int expectedNumQueenCaptures = 1;

    MoveList_t moveList;
    CapturesMovegen(&moveList, &info, &stack, white);

    bool success = 
        (CountPieceMoves(king, moveList, &info) == expectedNumKingCaptures) &&
        (CountPieceMoves(pawn, moveList, &info) == expectedNumPawnCaptures) &&
        (CountPieceMoves(rook, moveList, &info) == expectedNumRookCaptures) &&
        (CountPieceMoves(bishop, moveList, &info) == expectedNumBishopCaptures) &&
        (CountPieceMoves(knight, moveList, &info) == expectedNumKnightsCaptures) &&
        (CountPieceMoves(queen, moveList, &info) == expectedNumQueenCaptures) &&
        moveList.maxIndex == 7;

    PrintResults(success);
    PrintMoveList(&moveList, &info);
}

static void ShouldCorrectlyEvaluateDoubleEnPassant() {
    TestSetup();
    BoardInfo_t info;
    InitDoubleEnPassantPosition(&info);

    int expectedNumPawnWhiteCaptures = 2;
    int expectedNumPawnBlackCaptures = 2;

    MoveList_t wMoveList;
    CapturesMovegen(&wMoveList, &info, &stack, white);

    MoveList_t bMoveList;
    CapturesMovegen(&bMoveList, &info, &stack, black);

    bool success = 
        (CountPieceMoves(pawn, wMoveList, &info) == expectedNumPawnWhiteCaptures) &&
        (CountPieceMoves(pawn, bMoveList, &info) == expectedNumPawnBlackCaptures) &&
        (wMoveList.maxIndex == 1) && (bMoveList.maxIndex == 1);

    PrintResults(success);
}

static void ShouldCorrectlyEvaluatePinnedEnPassant() {
    TestSetup();
    BoardInfo_t info;
    InitTrickyPinnedEnPassantPostitionInfo(&info);

    int expectedNumPawnWhiteCaptures = 1;

    MoveList_t moveList;
    CapturesMovegen(&moveList, &info, &stack, white);

    bool success = 
        (CountPieceMoves(pawn, moveList, &info) == expectedNumPawnWhiteCaptures) &&
        moveList.maxIndex == 0;

    PrintResults(success);
}

static void ShouldCorrectlyEvaluateInPosWithPins() {
    TestSetup();
    BoardInfo_t info;
    InitPinPositionInfo(&info);
    
    int expectedNumKingMoves = 4;
    int expectedNumPawnMoves = 9;
    int expectedNumRookMoves = 3;
    int expectedNumBishopMoves = 7;
    int expectedNumKnightsMoves = 8;
    int expectedNumQueenMoves = 2;

    MoveList_t moveList;
    CompleteMovegen(&moveList, &info, &stack, white);

    int expectedMaxIndex = (
        expectedNumKingMoves +
        expectedNumPawnMoves +
        expectedNumRookMoves +
        expectedNumBishopMoves +
        expectedNumKnightsMoves +
        expectedNumQueenMoves -1);

    bool success = 
        (CountPieceMoves(king, moveList, &info) == expectedNumKingMoves) &&
        (CountPieceMoves(pawn, moveList, &info) == expectedNumPawnMoves) &&
        (CountPieceMoves(rook, moveList, &info) == expectedNumRookMoves) &&
        (CountPieceMoves(bishop, moveList, &info) == expectedNumBishopMoves) &&
        (CountPieceMoves(knight, moveList, &info) == expectedNumKnightsMoves) &&
        (CountPieceMoves(queen, moveList, &info) == expectedNumQueenMoves) &&
        moveList.maxIndex == expectedMaxIndex;

    PrintResults(success);
}

void MovegenTDDRunner() {
    ShouldCorrectlyEvaluateCapturesInPosWithPins();
    ShouldCorrectlyEvaluateDoubleEnPassant();
    ShouldCorrectlyEvaluatePinnedEnPassant();
    ShouldCorrectlyEvaluateInPosWithPins();
}