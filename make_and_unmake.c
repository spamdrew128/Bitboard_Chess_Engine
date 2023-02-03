#include <stdbool.h>
#include <assert.h>

#include "make_and_unmake.h"
#include "game_state.h"
#include "lookup.h"

enum {
    pawn_start_ranks = rank_2 | rank_7,
    pawn_double_ranks = rank_4 | rank_5,
    rook_start_squares = board_corners
};

static bool PawnIsDoublePushed(Bitboard_t fromBB, Bitboard_t toBB) {
    return (fromBB & pawn_start_ranks) && (toBB & pawn_double_ranks);
}

static void UpdateCastleSquares(GameState_t* nextState, BoardInfo_t* info, Color_t color) {
    Bitboard_t rooksInPlace = board_corners & info->rooks[color];
    Bitboard_t validCastlingMask = GenShiftWest(rooksInPlace, 1) | GenShiftEast(rooksInPlace, 2);
    
    nextState->castleSquares[color] &= validCastlingMask;
}

static void RemoveCapturedPiece(
    BoardInfo_t* boardInfo,
    Square_t capturedSquare,
    Piece_t type,
    Color_t capturedPieceColor
) 
{
    Bitboard_t capturedBB = GetSingleBitset(capturedSquare);

    ResetBits(GetPieceInfoField(boardInfo, type, capturedPieceColor), capturedBB);

    ResetBits(&(boardInfo->allPieces[capturedPieceColor]), capturedBB);
}

static void RemoveCapturedEnPassant(
    BoardInfo_t* boardInfo,
    Bitboard_t enPassantBB,
    Color_t capturedPieceColor
)
{
    ResetBits(&(boardInfo->pawns[capturedPieceColor]), enPassantBB);
    ResetBits(&(boardInfo->allPieces[capturedPieceColor]), enPassantBB);
    RemovePieceFromMailbox(boardInfo, LSB(enPassantBB));
}

static void UpdateBoardInfoField(
    BoardInfo_t* boardInfo,
    Bitboard_t* infoField,
    Bitboard_t fromBB,
    Bitboard_t toBB,
    Square_t fromSquare,
    Square_t toSquare,
    Color_t color
) 
{
    ResetBits(infoField, fromBB);
    SetBits(infoField, toBB);

    ResetBits(&(boardInfo->allPieces[color]), fromBB);
    SetBits(&(boardInfo->allPieces[color]), toBB);

    MovePieceInMailbox(boardInfo, toSquare, fromSquare);
}

static void MakeCastlingHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    Square_t kingFromSquare = ReadFromSquare(move);
    Square_t kingToSquare = ReadToSquare(move);

    Bitboard_t kingFromBB = boardInfo->kings[color];
    Bitboard_t kingToBB = GetSingleBitset(kingToSquare);

    UpdateBoardInfoField(
        boardInfo,
        &(boardInfo->kings[color]),
        kingFromBB,
        kingToBB,
        kingFromSquare,
        kingToSquare,
        color
    );

    if(kingToSquare < kingFromSquare) { // queenside castle
        Bitboard_t rookFromBB = GenShiftWest(kingFromBB, 4);
        Bitboard_t rookToBB = GenShiftWest(kingFromBB, 1);
        UpdateBoardInfoField(
            boardInfo,
            &(boardInfo->rooks[color]),
            rookFromBB,
            rookToBB,
            LSB(rookFromBB),
            LSB(rookToBB),
            color
        );

    } else {
        Bitboard_t rookFromBB = GenShiftEast(kingFromBB, 3);
        Bitboard_t rookToBB = GenShiftEast(kingFromBB, 1);
        UpdateBoardInfoField(
            boardInfo,
            &(boardInfo->rooks[color]),
            rookFromBB,
            rookToBB,
            LSB(rookFromBB),
            LSB(rookToBB),
            color
        );
    }

    GameState_t* nextState = GetDefaultNextGameState();
    nextState->castleSquares[color] = empty_set;
}

static void MakePromotionHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    Square_t fromSquare = ReadFromSquare(move);
    Square_t toSquare = ReadToSquare(move);
    Piece_t promotionPiece = ReadPromotionPiece(move);
    GameState_t* nextState = GetDefaultNextGameState();

    Piece_t capturedPiece = PieceOnSquare(boardInfo, toSquare);
    if(capturedPiece != none_type) {
        RemoveCapturedPiece(
            boardInfo,
            toSquare,
            capturedPiece,
            !color
        );

        nextState->capturedPiece = capturedPiece;
        UpdateCastleSquares(nextState, boardInfo, !color);
    }

    AddPieceToMailbox(boardInfo, fromSquare, promotionPiece);
    MovePieceInMailbox(boardInfo, toSquare, fromSquare);

    Bitboard_t fromBB = GetSingleBitset(fromSquare);
    Bitboard_t toBB = GetSingleBitset(toSquare);

    ResetBits(&(boardInfo->pawns[color]), fromBB);
    ResetBits(&(boardInfo->allPieces[color]), fromBB);
    SetBits(&(boardInfo->allPieces[color]), toBB);

    SetBits(GetPieceInfoField(boardInfo, promotionPiece, color), toBB);

    nextState->halfmoveClock = empty_set;
}

static Bitboard_t GetEnPassantBB(Bitboard_t toBB, Color_t color) {
    if(color == white) {
        return SoutOne(toBB);
    } else {
        return NortOne(toBB);
    }
}

static void MakeEnPassantHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    Square_t fromSquare = ReadFromSquare(move);
    Square_t toSquare = ReadToSquare(move);
    Bitboard_t fromBB = GetSingleBitset(fromSquare);
    Bitboard_t toBB = GetSingleBitset(toSquare);
    Bitboard_t enPassantBB = GetEnPassantBB(toBB, color);

    RemoveCapturedEnPassant(
        boardInfo,
        enPassantBB,
        !color
    );

    UpdateBoardInfoField(
        boardInfo,
        &(boardInfo->pawns[color]),
        fromBB,
        toBB,
        fromSquare,
        toSquare,
        color
    );

    GameState_t* nextState = GetDefaultNextGameState();
    nextState->halfmoveClock = empty_set;
    nextState->enPassantSquares = empty_set;
    nextState->capturedPiece = pawn;
}

static void MakeMoveDefaultHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    Square_t fromSquare = ReadFromSquare(move);
    Square_t toSquare = ReadToSquare(move);
    Bitboard_t fromBB = GetSingleBitset(fromSquare);
    Bitboard_t toBB = GetSingleBitset(toSquare);
    GameState_t* nextState = GetDefaultNextGameState();

    Piece_t capturedPiece = PieceOnSquare(boardInfo, toSquare);
    if(capturedPiece != none_type) {
        RemoveCapturedPiece(
            boardInfo,
            toSquare,
            capturedPiece,
            !color
        );

        nextState->halfmoveClock = 0;
        nextState->capturedPiece = capturedPiece;
        UpdateCastleSquares(nextState, boardInfo, !color); // if we captured, we might have messed up our opponent's castling rights
    }

    Piece_t type = PieceOnSquare(boardInfo, fromSquare);
    switch (type) {
        case pawn:
            nextState->halfmoveClock = 0;

            if(PawnIsDoublePushed(fromBB, toBB)) {
                nextState->enPassantSquares = GetEnPassantBB(toBB, color);
            }
        break;
        case king:
            nextState->castleSquares[color] = empty_set;
        break;
    }

    UpdateBoardInfoField(
        boardInfo,
        GetPieceInfoField(boardInfo, type, color),
        fromBB,
        toBB,
        fromSquare,
        toSquare,
        color
    );

    UpdateCastleSquares(nextState, boardInfo, color);
}

void MakeMove(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    SpecialFlag_t specialFlag = ReadSpecialFlag(move);
    
    switch (specialFlag) {
        case castle_flag:
            MakeCastlingHandler(boardInfo, move, color);
        break;
        case promotion_flag:
            MakePromotionHandler(boardInfo, move, color);
        break;
        case en_passant_flag:
            MakeEnPassantHandler(boardInfo, move, color);
        break;
        default:
            MakeMoveDefaultHandler(boardInfo, move, color);
        break;
    }

    UpdateEmpty(boardInfo);
}

static void RevertPieceCapture(
    BoardInfo_t* boardInfo,
    Square_t capturedSquare,
    Piece_t type,
    Color_t capturedPieceColor
) 
{
    Bitboard_t capturedBB = GetSingleBitset(capturedSquare);

    SetBits(GetPieceInfoField(boardInfo, type, capturedPieceColor), capturedBB);

    SetBits(&(boardInfo->allPieces[capturedPieceColor]), capturedBB);
    AddPieceToMailbox(boardInfo, capturedSquare, type);
}

static void UnmakeCastlingHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    Square_t kingOriginalSquare = ReadFromSquare(move);
    Square_t kingCurrentSquare = ReadToSquare(move);
    Bitboard_t kingOriginalBB = GetSingleBitset(kingOriginalSquare);
    Bitboard_t kingCurrentBB = GetSingleBitset(kingCurrentSquare);       

    UpdateBoardInfoField(
        boardInfo,
        &(boardInfo->kings[color]),
        kingCurrentBB,
        kingOriginalBB,
        kingCurrentSquare,
        kingOriginalSquare,
        color
    );

    if(kingCurrentSquare < kingOriginalSquare) { // queenside castle)
        Bitboard_t rookCurrentBB = GenShiftWest(kingOriginalBB, 1);
        Bitboard_t rookOriginalBB = GenShiftWest(kingOriginalBB, 4);

        UpdateBoardInfoField(
            boardInfo,
            &(boardInfo->rooks[color]),
            rookCurrentBB,
            rookOriginalBB,
            LSB(rookCurrentBB),
            LSB(rookOriginalBB),
            color
        );
    } else {
        Bitboard_t rookCurrentBB = GenShiftEast(kingOriginalBB, 1);
        Bitboard_t rookOriginalBB = GenShiftEast(kingOriginalBB, 3);

        UpdateBoardInfoField(
            boardInfo,
            &(boardInfo->rooks[color]),
            rookCurrentBB,
            rookOriginalBB,
            LSB(rookCurrentBB),
            LSB(rookOriginalBB),
            color
        );
    }
}

static void UnmakePromotionHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    Square_t originalSquare = ReadFromSquare(move);
    Square_t currentSquare = ReadToSquare(move);
    Bitboard_t originalBB = GetSingleBitset(originalSquare);
    Bitboard_t currentBB = GetSingleBitset(currentSquare);
    Piece_t promotionPiece = ReadPromotionPiece(move);
    Piece_t capturedPiece = ReadCapturedPiece(move);

    RemoveCapturedPiece( // treats the promoted piece as if it is captured
        boardInfo,
        currentSquare,
        promotionPiece,
        color
    );

    AddPieceToMailbox(
        boardInfo,
        currentSquare,
        pawn
    );

    UpdateBoardInfoField(
        boardInfo,
        &(boardInfo->pawns[color]),
        currentBB,
        originalBB,
        currentSquare,
        originalSquare,
        color
    );

    if(capturedPiece != none_type) {
        RevertPieceCapture(
            boardInfo,
            currentSquare,
            capturedPiece,
            !color
        );
    }
}

static void UnmakeEnPassantHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {

}

void UnmakeMove(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    SpecialFlag_t specialFlag = ReadSpecialFlag(move);
    
    switch (specialFlag) {
        case castle_flag:
            UnmakeCastlingHandler(boardInfo, move, color);
        break;
        case promotion_flag:
            UnmakePromotionHandler(boardInfo, move, color);
        break;
        case en_passant_flag:
            // UnmakeEnPassantHandler(boardInfo, move, color);
        break;
        default:
            // UnmakeMoveDefaultHandler(boardInfo, move, color);
        break;
    }

    UpdateEmpty(boardInfo);
    RevertState();
}