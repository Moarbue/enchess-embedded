#include "movement.h"
#include "enchess_pinout.h"

#define TMC2209_DEFAULT_STEPS_PER_REVOLUTION ENCHESS_STEPS_PER_REVOLUTION
#include "tmc2209/tmc2209.h"

#define ENCHESS_SQUARE_SIZE         (ENCHESS_BOARD_SIZE / 8) // size of one square in mm
#define ENCHESS_DEGREES_PER_SQUARE  (360.0 * (ENCHESS_SQUARE_SIZE / ENCHESS_THREADED_ROD_LEAD))
#define ENCHESS_DEGREES_PER_MM      (360.0 * (1.0 / ENCHESS_THREADED_ROD_LEAD))

tmc2209_t *s_col = NULL, *s_row = NULL;

// TODO: implement sensorless homing
Columns current_col = COLUMN_A;
Rows    current_row = ROW_1;

static void init_steppers(void)
{
    tmc2209_full(s_col, ENCHESS_PIN_S1_EN, ENCHESS_PIN_S1_DIR, ENCHESS_PIN_S1_STEP, ENCHESS_PIN_S_RX,
                        ENCHESS_PIN_S_TX,  ENCHESS_PIN_S1_MS1, ENCHESS_PIN_S1_MS2,  TMC2209_ADDRESS_0);
    tmc2209_set_microsteps(s_col, (tmc2209_microstep)ENCHESS_MICROSTEPS);

    tmc2209_full(s_row, ENCHESS_PIN_S2_EN, ENCHESS_PIN_S1_DIR, ENCHESS_PIN_S1_STEP, ENCHESS_PIN_S_RX,
                        ENCHESS_PIN_S_TX,  ENCHESS_PIN_S2_MS1, ENCHESS_PIN_S1_MS2,  TMC2209_ADDRESS_1);
    tmc2209_set_microsteps(s_row, (tmc2209_microstep)ENCHESS_MICROSTEPS);
}

static void home_routine(uint32_t step_delay, uint32_t retraction)
{
    bool x_stall, y_stall;
    uint32_t retracted_degrees;

    x_stall = y_stall = false;
    tmc2209_set_step_delay(s_col, step_delay);
    tmc2209_set_step_delay(s_row, step_delay);
    tmc2209_rotate(s_col, ENCHESS_BOARD_SIZE * ENCHESS_DEGREES_PER_MM);
    tmc2209_rotate(s_row, ENCHESS_BOARD_SIZE * ENCHESS_DEGREES_PER_MM);
    while (!x_stall && !y_stall) {
        if (!x_stall) {
            x_stall = tmc2209_is_stalling(s_col);
            if (x_stall) tmc2209_step_reset(s_col);
            else         tmc2209_update(s_col);
        }

        if (!y_stall) {
            y_stall = tmc2209_is_stalling(s_row);
            if (y_stall) tmc2209_step_reset(s_row);
            else         tmc2209_update(s_row);
        }

        // don't block RTOS
        yield();
        delayMicroseconds(10);
    }
    
    retracted_degrees = retraction * ENCHESS_DEGREES_PER_MM;
    tmc2209_rotate(s_col, -retracted_degrees);
    tmc2209_rotate(s_row, -retracted_degrees);
    while(!tmc2209_step_is_idle(s_col) && !tmc2209_step_is_idle(s_row)) {
        tmc2209_update(s_col); 
        tmc2209_update(s_row); 
        // don't block RTOS
        yield();
        delayMicroseconds(10);
    }
}

void home_motors(void)
{
    if (s_col == NULL || s_row == NULL) {
        init_steppers();
    }

    home_routine(ENCHESS_HOME_STEP_DELAY_1, ENCHESS_HOME_RETRACTION);
    home_routine(ENCHESS_HOME_STEP_DELAY_2, ENCHESS_HOME_RETRACTION);
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

    tmc2209_rotate(s_col, dx * ENCHESS_DEGREES_PER_SQUARE);
    tmc2209_rotate(s_row, dy * ENCHESS_DEGREES_PER_SQUARE);
}

void steppers_update(void)
{
    tmc2209_update(s_col);
    tmc2209_update(s_row);
}