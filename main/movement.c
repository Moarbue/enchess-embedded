#include "movement.h"
#include "enchess_pinout.h"
#include "util.h"

#define TMC2209_DEFAULT_STEPS_PER_REVOLUTION ENCHESS_STEPS_PER_REVOLUTION
#include "tmc2209.h"

#define ENCHESS_SQUARE_SIZE         (ENCHESS_BOARD_SIZE / 8) // size of one square in mm
#define ENCHESS_DEGREES_PER_SQUARE  (360.0 * (ENCHESS_SQUARE_SIZE / ENCHESS_THREADED_ROD_LEAD))
#define ENCHESS_DEGREES_PER_MM      (360.0 / ENCHESS_THREADED_ROD_LEAD)

tmc2209_t *s_col = NULL, *s_row = NULL;

Columns current_col = COLUMN_A;
Rows    current_row = ROW_1;

static void init_steppers(void)
{
    s_col = (tmc2209_t*) malloc(sizeof (tmc2209_t));
    tmc2209_full(s_col, ENCHESS_PIN_S1_EN, ENCHESS_PIN_S1_DIR, ENCHESS_PIN_S1_STEP,
                       ENCHESS_PIN_S_RX,  ENCHESS_PIN_S_TX,   ENCHESS_PIN_S1_MS1, ENCHESS_PIN_S1_MS2, TMC2209_ADDRESS_0, 0.11f);
    s_row = (tmc2209_t*) malloc(sizeof (tmc2209_t));
    tmc2209_full(s_row, ENCHESS_PIN_S2_EN, ENCHESS_PIN_S2_DIR, ENCHESS_PIN_S2_STEP,
                       ENCHESS_PIN_S_RX,  ENCHESS_PIN_S_TX,   ENCHESS_PIN_S2_MS1, ENCHESS_PIN_S2_MS2, TMC2209_ADDRESS_1, 0.11f);
    
    if (!tmc2209_check_connection(s_col)) {
        LOG_MSG("Failed to setup UART communication with S1!");
        delay(500);
        abort();
    }
    LOG_MSG("Set up UART communication with S1!");
    if (!tmc2209_check_connection(s_row)) {
        LOG_MSG("Failed to setup UART communication with S2!");
        delay(500);
        abort();
    }
    LOG_MSG("Set up UART communication with S2!");

    // s_col configuration
    tmc2209_enable(s_col);
    tmc2209_toff(s_col, 4);
    tmc2209_blank_time(s_col, 24);
    tmc2209_rms_current(s_col, 1000);
    tmc2209_set_microsteps(s_col, ENCHESS_MICROSTEPS);
    tmc2209_tcoolthrs(s_col, 0xFFFFF);
    tmc2209_semin(s_col, 5);
    tmc2209_semax(s_col, 2);
    tmc2209_sedn(s_col, 0b01);
    tmc2209_stallguard_thrs(s_col, ENCHESS_STALLGUARD_THRS);
    
    // s_row configuration
    tmc2209_enable(s_row);
    tmc2209_toff(s_row, 4);
    tmc2209_blank_time(s_row, 24);
    tmc2209_rms_current(s_row, 400);
    tmc2209_set_microsteps(s_row, ENCHESS_MICROSTEPS);
    tmc2209_tcoolthrs(s_row, 0xFFFFF);
    tmc2209_semin(s_row, 5);
    tmc2209_semax(s_row, 2);
    tmc2209_sedn(s_row, 0b01);
    tmc2209_stallguard_thrs(s_row, ENCHESS_STALLGUARD_THRS);
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
        }

        if (!y_stall) {
            y_stall = tmc2209_is_stalling(s_row);
            if (y_stall) tmc2209_step_reset(s_row);
        }

        // don't block RTOS
        yield();
        delayMicroseconds(10);
    }
    
    retracted_degrees = retraction * ENCHESS_DEGREES_PER_MM;
    tmc2209_rotate(s_col, -retracted_degrees);
    tmc2209_rotate(s_row, -retracted_degrees);
}

void home_motors(void)
{
    if (s_col == NULL || s_row == NULL) {
        init_steppers();
    }

    LOG_MSG("Homing...");

    home_routine(ENCHESS_STEP_DELAY,      ENCHESS_HOME_RETRACTION);
    home_routine(ENCHESS_HOME_STEP_DELAY, ENCHESS_HOME_RETRACTION);
    // find first square (A1)
    tmc2209_set_step_delay(s_col, ENCHESS_STEP_DELAY);
    tmc2209_set_step_delay(s_row, ENCHESS_STEP_DELAY);
    tmc2209_rotate(s_col, ((ENCHESS_SQUARE_SIZE / 2) - ENCHESS_HOME_RETRACTION + ENCHESS_BOARD_OFFSET_X) * ENCHESS_DEGREES_PER_MM);
    tmc2209_rotate(s_row, ((ENCHESS_SQUARE_SIZE / 2) - ENCHESS_HOME_RETRACTION + ENCHESS_BOARD_OFFSET_Y) * ENCHESS_DEGREES_PER_MM);
}

void execute_move(Columns c, Rows r)
{
    if (s_col == NULL || s_row == NULL) {
        init_steppers();
    }

    LOG_MSG("Executing Move: %c%c --> %c%c", (char)('A' + current_col), (char)('0' + current_row),
                                             (char)('A' + c),           (char)('0' + r));

    int8_t dx = current_col - c;
    int8_t dy = current_row - r;

    current_col = c;
    current_row = r;

    tmc2209_rotate(s_col, dx * ENCHESS_DEGREES_PER_SQUARE);
    tmc2209_rotate(s_row, dy * ENCHESS_DEGREES_PER_SQUARE);
}