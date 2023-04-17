#include "config.h"
#include "stdbool.h"

#ifndef ENCHESS_MOVEMENT_H
#define ENCHESS_MOVEMENT_H

#ifdef __cplusplus
extern "C"{
#endif


typedef enum {
    COLUMN_A = 0,
    COLUMN_B,
    COLUMN_C,
    COLUMN_D,
    COLUMN_E,
    COLUMN_F,
    COLUMN_G,
    COLUMN_H,
} Columns;

typedef enum {
    ROW_1 = 0,
    ROW_2,
    ROW_3,
    ROW_4,
    ROW_5,
    ROW_6,
    ROW_7,
    ROW_8,
} Rows;

void setup_motors(void);
void home_motors(void);
void execute_move(Columns c, Rows r, bool el_mag);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // ENCHESS_MOVEMENT_H