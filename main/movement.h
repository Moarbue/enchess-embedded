#include "config.h"

#ifndef ENCHESS_MOVEMENT_H
#define ENCHESS_MOVEMENT_H

#ifdef __cplusplus
extern "C"{
#endif


#define ENCHESS_SQAURE_SIZE       (ENCHESS_BOARD_SIZE / 8) // size of one square in mm
#define ENCHESS_DEGREES_PER_SQAURE  (360.0 * (ENCHESS_SQAURE_SIZE / ENCHESS_THREADED_ROD_LEAD))

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

void execute_move(Columns c, Rows r);
void steppers_update(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // ENCHESS_MOVEMENT_H