#pragma once
#define KING 1
#define QUEEN 2
#define ROOK 3
#define BISHOP 4
#define KNIGHT 5
#define PAWN 6

#define BLACK 64
#define WHITE 128

#define FIGURE(f) ((f) & 7)
#define COLOR(f) ((f) & (BLACK | WHITE))

#define MATE 100
