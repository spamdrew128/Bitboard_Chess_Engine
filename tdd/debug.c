#include <stdio.h>
#include <stdarg.h>

#include "debug.h"

static void FillBoardArray(char boardArray[], Bitboard_t b, char fillChar) {
    while(b) {
        Square_t square = __builtin_ctzll(b);
        boardArray[square] = fillChar;
        ResetLSB(&b);
    }
}

void PrintBitboard(Bitboard_t b) {
    Square_t boardArray[64] = {0};

    while(b) {
        Square_t square = __builtin_ctzll(b);
        boardArray[square] = 1;
        ResetLSB(&b);
    }

    printf("\n");
    for(int i = 7; i >= 0; i--) {
        for(int j = 0; j < 8; j++) {
            Square_t square = boardArray[i*8 + j];
            if(square) {
                printf("%d ", boardArray[i*8 + j]);
            } else {
                printf(". ");
            }
        }    
        printf("\n");
    }
}

void PrintMailbox(BoardInfo_t *info) {
    printf("\n");
    for(int i = 7; i >= 0; i--) {
        for(int j = 0; j < 8; j++) {
            Piece_t piece = info->mailbox[i*8 + j];

            switch (piece)
            {
            case queen:
                printf("Q ");
                break;
            case rook:
                printf("R ");
                break;
            case bishop:
                printf("B ");
                break;
            case knight:
                printf("N ");
                break;
            case king:
                printf("K ");
                break;
            case pawn:
                printf("P ");
                break;
            default:
                printf(". ");
                break;
            }
        }    
        printf("\n");
    }
}


void PrintChessboard(BoardInfo_t* info) {
    char boardArray[64] = {0};
    FillBoardArray(boardArray, full_set, '.');

    FillBoardArray(boardArray, info->rooks[white], 'R');
    FillBoardArray(boardArray, info->knights[white], 'N');
    FillBoardArray(boardArray, info->bishops[white], 'B');
    FillBoardArray(boardArray, info->queens[white], 'Q');
    FillBoardArray(boardArray, info->kings[white], 'K');
    FillBoardArray(boardArray, info->pawns[white], 'P');

    FillBoardArray(boardArray, info->rooks[black], 'r');
    FillBoardArray(boardArray, info->knights[black], 'n');
    FillBoardArray(boardArray, info->bishops[black], 'b');
    FillBoardArray(boardArray, info->queens[black], 'q');
    FillBoardArray(boardArray, info->kings[black], 'k');
    FillBoardArray(boardArray, info->pawns[black], 'p');

    printf("\n");
    for(int i = 7; i >= 0; i--) {
        for(int j = 0; j < 8; j++) {
            printf("%c ", boardArray[i*8 + j]);
        }    
        printf("\n");
    }
}

Bitboard_t CreateBitboard(int numOccupied, ...) {
    va_list valist;
    va_start(valist, numOccupied);

    Bitboard_t bitboard = empty_set;
    for (int i = 0; i < numOccupied; i++) {
        SetBits(&bitboard, C64(1) << va_arg(valist, int));
    } 

    return bitboard;
}

static char RowToNumber(int row) {
    return (char)(row + 49);
}

static char ColToLetter(int col) {
    return (char)(col + 97);
}

static void SquareToString(Square_t square, char string[3]) {
    int row = square / 8;
    int col = square % 8;

    string[0] = ColToLetter(col);
    string[1] = RowToNumber(row);
    string[2] = '\0';
}

static void PrintSingleTypeMoves(MoveList_t* moveList, BoardInfo_t* info, Piece_t type, const char* typeText) {
    char fromText[3];
    char toText[3];
    for(int i = 0; i <= moveList->maxIndex; i++) {
        Move_t current = moveList->moves[i];

        if(PieceOnSquare(info, ReadFromSquare(current)) == type) {
            SquareToString(ReadFromSquare(current), fromText);
            SquareToString(ReadToSquare(current), toText);
            printf("%s, From %s To %s\n", typeText, fromText, toText);
        }
    }
}

void PrintMoveList(MoveList_t* moveList, BoardInfo_t* info) {
    printf("\n");
    PrintSingleTypeMoves(moveList, info, king, "King");
    PrintSingleTypeMoves(moveList, info, queen, "Queen");
    PrintSingleTypeMoves(moveList, info, rook, "Rook");
    PrintSingleTypeMoves(moveList, info, bishop, "Bishop");
    PrintSingleTypeMoves(moveList, info, knight, "Knight");
    PrintSingleTypeMoves(moveList, info, pawn, "Pawn");
}

void AddGameStateToStack(GameState_t stateToAdd) {
    GameState_t* gameState = GetUninitializedNextGameState();
    *gameState = stateToAdd;
}