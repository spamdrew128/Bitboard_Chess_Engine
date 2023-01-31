#include "make_and_unmake.h"
#include "game_state.h"
#include "lookup.h"

static void RemoveCapturedPiece(
    BoardInfo_t* boardInfo,
    Square_t capturedSquare,
    Piece_t type,
    Color_t capturedPieceColor
) 
{
    Bitboard_t caputuredBB = GetSingleBitset(capturedSquare);

    switch (type) {
        case queen:
            ResetBits(&(boardInfo->queens[capturedPieceColor]), caputuredBB);
        break;
        case rook:
            ResetBits(&(boardInfo->rooks[capturedPieceColor]), caputuredBB);
        break;
        case bishop:
            ResetBits(&(boardInfo->bishops[capturedPieceColor]), caputuredBB);
        break;
        case knight:
            ResetBits(&(boardInfo->knights[capturedPieceColor]), caputuredBB);
        break;
        case pawn:
            ResetBits(&(boardInfo->pawns[capturedPieceColor]), caputuredBB);
        break;
    }

    ResetBits(&(boardInfo->allPieces[capturedPieceColor]), caputuredBB);
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

    if(kingToSquare < kingFromSquare) { // queenside castle
        UpdateBoardInfoField(
            boardInfo,
            &(boardInfo->kings[color]),
            kingFromBB,
            GenShiftWest(kingFromBB, 2),
            kingFromSquare,
            kingToSquare,
            color
        );

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
        UpdateBoardInfoField(
            boardInfo,
            &(boardInfo->kings[color]),
            kingFromBB,
            GenShiftEast(kingFromBB, 2),
            kingFromSquare,
            kingToSquare,
            color
        );

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

    UpdateEmpty(boardInfo);
}

static void MakePromotionHandler(BoardInfo_t* boardInfo, Move_t move, Color_t color) {
    Square_t fromSquare = ReadFromSquare(move);
    Square_t toSquare = ReadToSquare(move);
    Piece_t promotionPiece = ReadPromotionPiece(move);

    Piece_t capturedPiece = PieceOnSquare(boardInfo, toSquare);
    if(capturedPiece != none_type) {
        RemoveCapturedPiece(
            boardInfo,
            toSquare,
            capturedPiece,
            !color
        );
    }

    AddPieceToMailbox(boardInfo, fromSquare, promotionPiece);
    MovePieceInMailbox(boardInfo, toSquare, fromSquare);

    Bitboard_t fromBB = GetSingleBitset(fromSquare);
    Bitboard_t toBB = GetSingleBitset(toSquare);

    ResetBits(&(boardInfo->pawns[color]), fromBB);
    ResetBits(&(boardInfo->allPieces[color]), fromBB);
    SetBits(&(boardInfo->allPieces[color]), toBB);

    switch (promotionPiece) {
        case queen:
            SetBits(&(boardInfo->queens[color]), toBB);
        break;
        case rook:
            SetBits(&(boardInfo->rooks[color]), toBB);
        break;
        case bishop:
            SetBits(&(boardInfo->bishops[color]), toBB);
        break;
        case knight:
            SetBits(&(boardInfo->knights[color]), toBB);
        break;
    }

    UpdateEmpty(boardInfo);

    GameState_t* nextState = GetDefaultNextGameState();
    nextState->halfmoveClock = empty_set;
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
        /* code */
        break;
        default:
        break;
    }
}