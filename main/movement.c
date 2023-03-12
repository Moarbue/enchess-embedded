#include "movement.h"
#include "enchess_pinout.h"

tmc2209_t *s_col = NULL, *s_row = NULL;

// TODO: implement sensorless homing
Columns current_col = COLUMN_A;
Rows    current_row = ROW_1;

static void init_steppers(void)
{
    tmc2209_new(s_col, ENCHESS_PIN_S1_DIR, ENCHESS_PIN_S1_STEP, ENCHESS_PIN_S1_EN);
    tmc2209_set_step_delay(s_col, ENCHESS_STEP_DELAY);
    tmc2209_set_steps_per_revolution(s_col, ENCHESS_STEPS_PER_REVOLUTION);
    tmc2209_set_microsteps(s_col, ENCHESS_MICROSTEPS);

    tmc2209_new(s_row, ENCHESS_PIN_S2_DIR, ENCHESS_PIN_S2_STEP, ENCHESS_PIN_S2_EN);
    tmc2209_set_step_delay(s_row, ENCHESS_STEP_DELAY);
    tmc2209_set_steps_per_revolution(s_row, ENCHESS_STEPS_PER_REVOLUTION);
    tmc2209_set_microsteps(s_row, ENCHESS_MICROSTEPS);
}

void execute_move(Columns c, Rows r)
{
    if (s_col == NULL || s_row == NULL) {
        init_steppers();
    }

    int8_t dx = current_col - c;
    int8_t dy = current_row - r;

    tmc2209_rotate(s_col, dx * ENCHESS_DEGREES_PER_SQAURE);
    tmc2209_rotate(s_row, dy * ENCHESS_DEGREES_PER_SQAURE);
}

void steppers_update(void)
{
    tmc2209_update(s_col);
    tmc2209_update(s_row);
}