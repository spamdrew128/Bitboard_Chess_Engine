#ifndef __PST_H__
#define __PST_H__

// inspired by https://www.chessprogramming.org/Simplified_Evaluation_Function
// all values are for black, becuase my scheme is flipped

#define PAWN_MG_PST { \
    0,  0,  0,  0,  0,  0,  0,  0, \
    50, 50, 50, 50, 50, 50, 50, 50, \
    10, 10, 20, 30, 30, 20, 10, 10, \
    0,  0, 10, 25, 25, 10,  0,  0, \
    0,  0,  0, 20, 20,  0,  0,  0, \
    0, 0, -10,  0,  0,-10, 5,  0, \
    5, 10, 10,-20,-20, 10, 10,  5, \
    0,  0,  0,  0,  0,  0,  0,  0, \
}

#define PAWN_EG_PST { \
    0,  0,  0,  0,  0,  0,  0,  0, \
    150, 150, 150, 150, 150, 150, 150, 150, \
    100, 80, 70, 70, 70, 80, 100, 100,\
    30, 30, 25, 20, 20, 25, 30, 30, \
    0,  0,  0, 20, 20,  0,  0,  0, \
    5, -5,-10,  0,  0,-10, -5,  5, \
    5, 10, 10,-20,-20, 10, 10,  5, \
    0,  0,  0,  0,  0,  0,  0,  0, \
}

#define KNIGHT_MG_PST { \
    -50,-40,-30,-30,-30,-30,-40,-50, \
    -40,-20,  0,  0,  0,  0,-20,-40, \
    -30,  0, 10, 15, 15, 10,  0,-30, \
    -30,  5, 15, 20, 20, 15,  5,-30, \
    -30,  0, 15, 20, 20, 15,  0,-30, \
    -30,  5, 10, 15, 15, 10,  5,-30, \
    -40,-20,  0,  5,  5,  0,-20,-40, \
    -50,-40,-30,-30,-30,-30,-40,-50, \
}

#define KNIGHT_EG_PST { \
    -50,-40,-30,-30,-30,-30,-40,-50, \
    -40,-20,  0,  0,  0,  0,-20,-40, \
    -30,  0, 10, 15, 15, 10,  0,-30, \
    -30,  5, 15, 20, 20, 15,  5,-30, \
    -30,  0, 15, 20, 20, 15,  0,-30, \
    -30,  5, 10, 15, 15, 10,  5,-30, \
    -40,-20,  0,  5,  5,  0,-20,-40, \
    -50,-40,-30,-30,-30,-30,-40,-50, \
}

#define BISHOP_MG_PST { \
    -20,-10,-10,-10,-10,-10,-10,-20, \
    -10,  0,  0,  0,  0,  0,  0,-10, \
    -10,  0,  5, 10, 10,  5,  0,-10, \
    -10,  5,  5, 10, 10,  5,  5,-10, \
    -10,  0, 10, 10, 10, 10,  0,-10, \
    -10, 5, 10, 10, 10, 10, 5, -10, \
    -10,  10,  0,  0,  0,  0, 10,-10, \
    -20,-10,-10,-10,-10,-10,-10,-20, \
}

#define BISHOP_EG_PST { \
    -20,-10,-10,-10,-10,-10,-10,-20, \
    -10,  0,  0,  0,  0,  0,  0,-10, \
    -10,  0,  5, 10, 10,  5,  0,-10, \
    -10,  5,  5, 10, 10,  5,  5,-10, \
    -10,  0, 10, 10, 10, 10,  0,-10, \
    -10, 10, 10, 10, 10, 10, 10,-10, \
    -10,  5,  0,  0,  0,  0,  5,-10, \
    -20,-10,-10,-10,-10,-10,-10,-20, \
}

#define ROOK_MG_PST { \
    0,  0,  0,  0,  0,  0,  0,  0, \
    5, 30, 30, 30, 30, 30, 30,  5, \
    -5, 10, 10, 10, 10, 10, 10, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  10, 10, 0,  0,  10, 10, -5, \
    0,  0,  0,  10,  10,  0,  0,  0, \
}

#define ROOK_EG_PST { \
    0,  0,  0,  0,  0,  0,  0,  0, \
    5, 30, 30, 30, 30, 30, 30,  5, \
    -5, 10, 10, 10, 10, 10, 10, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  10, 10, 0,  0,  10, 10, -5, \
    0,  0,  0,  10,  10,  0,  0,  0, \
}
#define QUEEN_MG_PST { \
    -20,-10,-10, -5, -5,-10,-10,-20, \
    -10,  0,  0,  0,  0,  0,  0,-10, \
    -10,  0,  5,  5,  5,  5,  0,-10, \
    -5,  0,  5,  5,  5,  5,  0, -5, \
     0,  0,  5,  5,  5,  5,  0, -5, \
    -10,  5,  5,  5,  5,  5,  0,-10, \
    -10,  0,  5,  0,  0,  0,  0,-10, \
    -20,-10,-10, -5, -5,-10,-10,-20, \
}

#define QUEEN_EG_PST { \
    -20,-10,-10, -5, -5,-10,-10,-20, \
    -10,  0,  0,  0,  0,  0,  0,-10, \
    -10,  0,  5,  5,  5,  5,  0,-10, \
    -5,  0,  5,  5,  5,  5,  0, -5, \
     0,  0,  5,  5,  5,  5,  0, -5, \
    -10,  5,  5,  5,  5,  5,  0,-10, \
    -10,  0,  5,  0,  0,  0,  0,-10, \
    -20,-10,-10, -5, -5,-10,-10,-20, \
}

#define KING_MG_PST { \
    0,  0,  0,  0,  0,  0,  0,  0, \
    5, 10, 10, 10, 10, 10, 10,  5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    -5,  0,  0,  0,  0,  0,  0, -5, \
    0,  0,  0,  5,  5,  0,  0,  0, \
}

#define KING_EG_PST { \
    -50,-40,-30,-20,-20,-30,-40,-50, \
    -30,-20,-10,  0,  0,-10,-20,-30, \
    -30,-10, 20, 30, 30, 20,-10,-30, \
    -30,-10, 30, 40, 40, 30,-10,-30, \
    -30,-10, 30, 40, 40, 30,-10,-30, \
    -30,-10, 20, 30, 30, 20,-10,-30, \
    -30,-30,  0,  0,  0,  0,-30,-30, \
    -50,-30,-30,-30,-30,-30,-30,-50, \
}

#endif