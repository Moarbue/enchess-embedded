#include "Arduino.h"

#ifndef ENCHESS_RECOGNITION_H
#define ENCHESS_RECOGNITION_H

#define SQUARES_COUNT 64
#define OCCUPIED 0
#define EMPTY    1

typedef enum {
    white_pawn = 0,
    white_king,
    white_queen,
    white_bishop,
    white_rook,
    white_knight,

    black_pawn,
    black_king,
    black_queen,
    black_bishop,
    black_rook,
    black_knight,

    none,
} Pieces;

void setup_sensors(void);

#endif // ENCHESS_RECOGNITION_H