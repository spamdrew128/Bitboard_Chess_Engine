#include <stdio.h>
#include <stdbool.h>

#include "debug.h"
#include "pieces_tdd.h"
#include "board_constants.h"
#include "bitboards.h"

enum {
    start_empty = rank_3 | rank_4 | rank_5 | rank_6,
    start_w = rank_2,
    start_b = rank_7,
    start_w_all = rank_1 | rank_2,
    start_b_all = rank_7 | rank_8,
    start_expected_w_single = rank_3,
    start_expected_b_single = rank_6,
    start_expected_w_double = rank_4,
    start_expected_b_double = rank_5,

    midgame_empty = 0x8A4C13A7EFDBD853,
    midgame_w = 0x4810002700,
    midgame_b = 0xA34C1000000000,

    other_midgame_w = 0x3001CE00,
    other_midgame_b = 0xE3001C00000000,
    other_midgame_w_all = 0x1003005CEDD,
    other_midgame_b_all = 0xBFE3003C00000000,

    en_passant_test_b_pawns = 0xE0100805000000,
};

// HELPERS
static void InitStartInfo(BoardInfo_t* info) {
    info->pawns[white] = rank_2;
    info->knights[white] = CreateBitboard(2, b1,g1);
    info->bishops[white] = CreateBitboard(2, c1,f1);
    info->rooks[white] = CreateBitboard(2, a1,h1);
    info->queens[white] = CreateBitboard(1, d1);
    info->kings[white] = CreateBitboard(1, e1);

    info->pawns[black] = rank_7;
    info->knights[black] = CreateBitboard(2, b8,g8);
    info->bishops[black] = CreateBitboard(2, c8,f8);
    info->rooks[black] = CreateBitboard(2, a8,h8);
    info->queens[black] = CreateBitboard(1, d8);
    info->kings[black] = CreateBitboard(1, e8);

    UpdateAllPieces(info);
    UpdateEmpty(info);
}

// r1b1qrk1/pp2np1p/2pp1npQ/3Pp1P1/4P3/2N2N2/PPP2P2/2KR1B1R
static void InitMidgameInfo(BoardInfo_t* info) {
    info->pawns[white] = CreateBitboard(7, a2,b2,c2,d5,e4,f2,g5);
    info->knights[white] = CreateBitboard(2, c3,f3);
    info->bishops[white] = CreateBitboard(1, f1);
    info->rooks[white] = CreateBitboard(2, d1,h1);
    info->queens[white] = CreateBitboard(1, h6);
    info->kings[white] = CreateBitboard(1, c1);

    info->pawns[black] = CreateBitboard(8, a7,b7,c6,d6,e5,f7,g6,h7);
    info->knights[black] = CreateBitboard(2, e7,f6);
    info->bishops[black] = CreateBitboard(1, c8);
    info->rooks[black] = CreateBitboard(2, a8,f8);
    info->queens[black] = CreateBitboard(1, e8);
    info->kings[black] = CreateBitboard(1, g8);

    UpdateAllPieces(info);
    UpdateEmpty(info);
}

// 5k1r/1ppb3p/1p1p3p/3B4/1P1p1P2/1Qp1q3/P5PP/3R1K2
static void InitOtherMidgameInfo(BoardInfo_t* info) {
    info->pawns[white] = CreateBitboard(5, a2,g2,h2,b4,f4);
    info->knights[white] = empty_set;
    info->bishops[white] = CreateBitboard(1, d5);
    info->rooks[white] = CreateBitboard(1, d1);
    info->queens[white] = CreateBitboard(1, b3);
    info->kings[white] = CreateBitboard(1, f1);

    info->pawns[black] = CreateBitboard(8, c3,d4,b6,d6,h6,b7,c7,h7);
    info->knights[black] = empty_set;
    info->bishops[black] = CreateBitboard(1, d7);
    info->rooks[black] = CreateBitboard(1, h8);
    info->queens[black] = CreateBitboard(1, e3);
    info->kings[black] = CreateBitboard(1, f8);

    UpdateAllPieces(info);
    UpdateEmpty(info);
}

// 8/8/7K/pP4p1/Pk6/8/6PP/8
static void InitEndgameInfo(BoardInfo_t* info) {
    info->pawns[white] = CreateBitboard(4, a4,b5,g2,h2);
    info->knights[white] = empty_set;
    info->bishops[white] = empty_set;
    info->rooks[white] = empty_set;
    info->queens[white] = empty_set;
    info->kings[white] = CreateBitboard(1, h6);

    info->pawns[black] = CreateBitboard(2, a5,g5);
    info->knights[black] = empty_set;
    info->bishops[black] = empty_set;
    info->rooks[black] = empty_set;
    info->queens[black] = empty_set;
    info->kings[black] = CreateBitboard(1, b4);

    UpdateAllPieces(info);
    UpdateEmpty(info);
}

// PAWN TESTS
static void StartSinglePawnPushesMatch() {
    bool success = 
        (WhiteSinglePushTargets(start_w, start_empty) == start_expected_w_single) &&
        (BlackSinglePushTargets(start_b, start_empty) == start_expected_b_single);
    PrintResults(success);
}

static void StartDoublePawnPushesMatch() {
    bool success = 
        (WhiteDoublePushTargets(start_w, start_empty) == start_expected_w_double) &&
        (BlackDoublePushTargets(start_b, start_empty) == start_expected_b_double);
    PrintResults(success);
}

static void MidgameSinglePawnPushesMatch() {
    Bitboard_t expectedWhite = CreateBitboard(2, a3,b3);
    Bitboard_t expectedBlack = CreateBitboard(3, a6,b6,c5);

    bool success = 
        (WhiteSinglePushTargets(midgame_w, midgame_empty) == expectedWhite) &&
        (BlackSinglePushTargets(midgame_b, midgame_empty) == expectedBlack);
    PrintResults(success);
}

static void MidgameDoublePawnPushesMatch() {
    Bitboard_t expectedWhite = CreateBitboard(2, a4,b4);
    Bitboard_t expectedBlack = CreateBitboard(2, a5,b5);

    bool success = 
        (WhiteDoublePushTargets(midgame_w, midgame_empty) == expectedWhite) &&
        (BlackDoublePushTargets(midgame_b, midgame_empty) == expectedBlack);
    PrintResults(success);
}

static void StartCaptureTargets() {
    bool success = 
        (WhiteEastCaptureTargets(start_w, start_b_all) == empty_set) &&
        (BlackEastCaptureTargets(start_b, start_w_all) == empty_set) &&
        (WhiteWestCaptureTargets(start_w, start_b_all) == empty_set) &&
        (BlackWestCaptureTargets(start_b, start_w_all) == empty_set);

    PrintResults(success);
}

static void OtherMidgameCaptureTargets() {
    Bitboard_t expectedWhiteEast = CreateBitboard(1, f5);
    Bitboard_t expectedBlackEast = CreateBitboard(2, e4,f4);
    Bitboard_t expectedWhiteWest = CreateBitboard(2, d5,e5);
    Bitboard_t expectedBlackWest = CreateBitboard(1, a6);

    bool success = 
        (WhiteEastCaptureTargets(other_midgame_w, other_midgame_b_all) == expectedWhiteEast) &&
        (BlackEastCaptureTargets(other_midgame_b, other_midgame_w_all) == expectedBlackEast) &&
        (WhiteWestCaptureTargets(other_midgame_w, other_midgame_b_all) == expectedWhiteWest) &&
        (BlackWestCaptureTargets(other_midgame_b, other_midgame_w_all) == expectedBlackWest);

    PrintResults(success);
}

static void EnPassantCaptureTargets() {
    Bitboard_t expectedBlackEast = CreateBitboard(1, b3);
    Bitboard_t expectedBlackWest = CreateBitboard(1, b3);
    Bitboard_t enPassantSquare = CreateBitboard(1, b3);

    bool success = 
        (BlackEastEnPassantTargets(en_passant_test_b_pawns, enPassantSquare) == expectedBlackEast) &&
        (BlackWestEnPassantTargets(en_passant_test_b_pawns, enPassantSquare) == expectedBlackWest);

    PrintResults(success);
}

// KNIGHT TESTS
static void StartKnightMoveTargets() {
    BoardInfo_t info;
    InitStartInfo(&info);

    Bitboard_t expectedB1Knight = CreateBitboard(2, a3,c3);
    Bitboard_t expectedB8Knight = CreateBitboard(2, a6,c6);
    Bitboard_t expectedG1Knight = CreateBitboard(2, f3,h3);
    Bitboard_t expectedG8Knight = CreateBitboard(2, f6,h6);

    bool success = 
        (KnightMoveTargets(b1, info.empty) == expectedB1Knight) &&
        (KnightMoveTargets(b8, info.empty) == expectedB8Knight) &&
        (KnightMoveTargets(g1, info.empty) == expectedG1Knight) &&
        (KnightMoveTargets(g8, info.empty) == expectedG8Knight);

    PrintResults(success);
}

static void StartKnightCaptureTargets() {
    BoardInfo_t info;
    InitStartInfo(&info);

    Bitboard_t expectedB1Knight = empty_set;
    Bitboard_t expectedB8Knight = empty_set;
    Bitboard_t expectedG1Knight = empty_set;
    Bitboard_t expectedG8Knight = empty_set;

    bool success = 
        (KnightMoveTargets(b1, info.allPieces[black]) == expectedB1Knight) &&
        (KnightMoveTargets(b8, info.allPieces[white]) == expectedB8Knight) &&
        (KnightMoveTargets(g1, info.allPieces[black]) == expectedG1Knight) &&
        (KnightMoveTargets(g8, info.allPieces[white]) == expectedG8Knight);

    PrintResults(success);
}

static void MidgameKnightMoveTargets() {
    BoardInfo_t info;
    InitMidgameInfo(&info);

    Bitboard_t expectedC3Knight = CreateBitboard(4, a4,b1,b5,e2);
    Bitboard_t expectedF3Knight = CreateBitboard(6, d2,d4,e1,g1,h2,h4);
    Bitboard_t expectedE7Knight = CreateBitboard(1, f5);
    Bitboard_t expectedF6Knight = CreateBitboard(3, d7,g4,h5);

    bool success = 
        (KnightMoveTargets(c3, info.empty) == expectedC3Knight) &&
        (KnightMoveTargets(f3, info.empty) == expectedF3Knight) &&
        (KnightMoveTargets(e7, info.empty) == expectedE7Knight) &&
        (KnightMoveTargets(f6, info.empty) == expectedF6Knight);

    PrintResults(success);
}

static void MidgameKnightCaptureTargets() {
    BoardInfo_t info;
    InitMidgameInfo(&info);
    
    Bitboard_t expectedC3Knight = empty_set;
    Bitboard_t expectedF3Knight = CreateBitboard(1, e5);
    Bitboard_t expectedE7Knight = CreateBitboard(1, d5);
    Bitboard_t expectedF6Knight = CreateBitboard(2, d5,e4);

    bool success = 
        (KnightMoveTargets(c3, info.allPieces[black]) == expectedC3Knight) &&
        (KnightMoveTargets(f3, info.allPieces[black]) == expectedF3Knight) &&
        (KnightMoveTargets(e7, info.allPieces[white]) == expectedE7Knight) &&
        (KnightMoveTargets(f6, info.allPieces[white]) == expectedF6Knight);

    PrintResults(success);
}

// KING TESTS
static void EndgameKingMoveTargets() {
    BoardInfo_t info;
    InitEndgameInfo(&info);

    Bitboard_t expectedWhiteKing = CreateBitboard(4, g6,g7,h5,h7);
    Bitboard_t expectedBlackKing = CreateBitboard(5, a3,b3,c3,c4,c5);

    bool success = 
        (KingMoveTargets(LSB(info.kings[white]), info.empty) == expectedWhiteKing) &&
        (KingMoveTargets(LSB(info.kings[black]), info.empty) == expectedBlackKing);

    PrintResults(success);
}

static void EndgameKingCaptureTargets() {
    BoardInfo_t info;
    InitEndgameInfo(&info);

    Bitboard_t expectedWhiteKing = CreateBitboard(1, g5);
    Bitboard_t expectedBlackKing = CreateBitboard(2, a4,b5);

    bool success = 
        (KingMoveTargets(LSB(info.kings[white]), info.allPieces[black]) == expectedWhiteKing) &&
        (KingMoveTargets(LSB(info.kings[black]), info.allPieces[white]) == expectedBlackKing);

    PrintResults(success);
}

static void OtherMidgameRookMoveTargets() {
    BoardInfo_t info;
    InitOtherMidgameInfo(&info);

    Square_t rookSquare = d1;
    Bitboard_t expectedD1RookMoves = CreateBitboard(6, a1,b1,c1,e1,d2,d3);

    bool success = RookMoveTargets(rookSquare, info.empty, info.empty) == expectedD1RookMoves;

    PrintResults(success);
}

static void OtherMidgameRookCaptureTargets() {
    BoardInfo_t info;
    InitOtherMidgameInfo(&info);

    Square_t rookSquare = d1;
    Bitboard_t expectedD1RookCaptures = CreateBitboard(1, d4);

    bool success = RookMoveTargets(rookSquare, info.empty, info.allPieces[black]) == expectedD1RookCaptures;

    PrintResults(success);
}

static void OtherMidgameBishopMoveTargets() {
    BoardInfo_t info;
    InitOtherMidgameInfo(&info);

    Square_t bishopSquare = d5;
    Bitboard_t expectedD5BishopMoves = CreateBitboard(7, f3,c4,e4,c6,e6,f7,g8);

    bool success = BishopMoveTargets(bishopSquare, info.empty, info.empty) == expectedD5BishopMoves;

    PrintResults(success);
}

static void OtherMidgameBishopCaptureTargets() {
    BoardInfo_t info;
    InitOtherMidgameInfo(&info);

    Square_t bishopSquare = d5;
    Bitboard_t expectedD5BishopCaptures = CreateBitboard(1, b7);

    bool success = BishopMoveTargets(bishopSquare, info.empty, info.allPieces[black]) == expectedD5BishopCaptures;

    PrintResults(success);
}

// k2n1n2/4P3/K7/8/8/8/2p5/3R4
static void FilterPawnPromotions() {
    Bitboard_t wPieces = CreateBitboard(3, d1,a6,e7);
    Bitboard_t bPieces = CreateBitboard(4, c2,a8,d8,f8);
    Bitboard_t wPawns = CreateBitboard(1, e7);
    Bitboard_t bPawns = CreateBitboard(1, c2);
    Bitboard_t empty = ~(wPieces | bPieces);

    Bitboard_t wPawnMoves = WhiteSinglePushTargets(wPawns, empty);
    Bitboard_t wPawnCaptures = WhiteEastCaptureTargets(wPawns, bPieces) | WhiteWestCaptureTargets(wPawns, bPieces);
    Bitboard_t bPawnMoves = BlackSinglePushTargets(bPawns, empty);
    Bitboard_t bPawnCaptures = BlackEastCaptureTargets(bPawns, wPieces) | BlackWestCaptureTargets(bPawns, wPieces);

    Bitboard_t expectedMoves = empty_set;
    Bitboard_t expectedCaptures = empty_set;
    Bitboard_t expectedWhitePromotions = CreateBitboard(3 ,d8,e8,f8);
    Bitboard_t expectedBlackPromotions = CreateBitboard(2 ,c1,d1);

    Bitboard_t wPromotions = FilterWhitePromotions(&wPawnMoves) | FilterWhitePromotions(&wPawnCaptures);
    Bitboard_t bPromotions = FilterBlackPromotions(&bPawnMoves) | FilterBlackPromotions(&bPawnCaptures);

    bool filteredCorrectly = 
        (wPawnMoves == expectedMoves) &&
        (wPawnCaptures == expectedCaptures) &&
        (bPawnMoves == expectedMoves) &&
        (bPawnCaptures == expectedCaptures);

    bool correctPromotions = 
        (wPromotions == expectedWhitePromotions) &&
        (bPromotions == expectedBlackPromotions);

    bool success = filteredCorrectly && correctPromotions;

    PrintResults(success);    
}

void PiecesTDDRunner() {
    StartSinglePawnPushesMatch();
    StartDoublePawnPushesMatch();
    MidgameSinglePawnPushesMatch();
    MidgameDoublePawnPushesMatch();

    StartCaptureTargets();
    OtherMidgameCaptureTargets();

    EnPassantCaptureTargets();

    StartKnightMoveTargets();
    StartKnightCaptureTargets();
    MidgameKnightMoveTargets();
    MidgameKnightCaptureTargets();

    EndgameKingMoveTargets();
    EndgameKingCaptureTargets();

    OtherMidgameRookMoveTargets();
    OtherMidgameRookCaptureTargets();
    OtherMidgameBishopMoveTargets();
    OtherMidgameBishopCaptureTargets();

    FilterPawnPromotions();
}