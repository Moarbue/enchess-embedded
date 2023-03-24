#include "movement.h"
#include "enchess_pinout.h"

#define TMC2209_DEFAULT_STEPS_PER_REVOLUTION ENCHESS_STEPS_PER_REVOLUTION
#include "tmc2209/tmc2209.h"

#define ENCHESS_SQAURE_SIZE       (ENCHESS_BOARD_SIZE / 8) // size of one square in mm
#define ENCHESS_DEGREES_PER_SQAURE  (360.0 * (ENCHESS_SQAURE_SIZE / ENCHESS_THREADED_ROD_LEAD))

tmc2209_t *s_col = NULL, *s_row = NULL;

// TODO: implement sensorless homing
Columns current_col = COLUMN_A;
Rows    current_row = ROW_1;

static void init_steppers(void)
{
    tmc2209_full(s_col, ENCHESS_PIN_S1_EN, ENCHESS_PIN_S1_DIR, ENCHESS_PIN_S1_STEP, ENCHESS_PIN_S_RX,
                        ENCHESS_PIN_S_TX,  ENCHESS_PIN_S1_MS1, ENCHESS_PIN_S1_MS2,  TMC2209_ADDRESS_0);
    tmc2209_set_step_delay(s_col, ENCHESS_STEP_DELAY);
    tmc2209_set_microsteps(s_col, (tmc2209_microstep)ENCHESS_MICROSTEPS);

    tmc2209_full(s_row, ENCHESS_PIN_S2_EN, ENCHESS_PIN_S1_DIR, ENCHESS_PIN_S1_STEP, ENCHESS_PIN_S_RX,
                        ENCHESS_PIN_S_TX,  ENCHESS_PIN_S2_MS1, ENCHESS_PIN_S1_MS2,  TMC2209_ADDRESS_1);
    tmc2209_set_step_delay(s_row, ENCHESS_STEP_DELAY);
    tmc2209_set_microsteps(s_row, (tmc2209_microstep)ENCHESS_MICROSTEPS);
}

void home_motors(void)
{
    while (!tmc2209_is_stalling(s_col)) {
        tmc2209_rotate(s, 1);
        tmc2209_update(s);
    }
}

void execute_move(Columns c, Rows r)
{
    if (s_col == NULL || s_row == NULL) {
        init_steppers();
    }

    int8_t dx = current_col - c;
    int8_t dy = current_row - r;

    current_col = c;
    current_row = r;

    tmc2209_rotate(s_col, dx * ENCHESS_DEGREES_PER_SQAURE);
    tmc2209_rotate(s_row, dy * ENCHESS_DEGREES_PER_SQAURE);
}

void steppers_update(void)
{
    tmc2209_update(s_col);
    tmc2209_update(s_row);
}