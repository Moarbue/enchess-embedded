#include "TMC2209.h"


void TMC2209_PREFIX(new)(tmc2209_t *stepper, uint8_t dir_pin, uint8_t step_pin, uint8_t en_pin)
{
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(en_pin, OUTPUT);

    digitalWrite(en_pin, LOW);

    tmc2209_t s = {0};
    s.dir_pin              = dir_pin;
    s.step_pin             = step_pin;
    s.en_pin               = en_pin;
    s.steps_per_revolution = TMC2209_DEFAULT_STEPS_PER_REVOLUTION;
    s.microsteps           = TMC2209_DEFAULT_MICROSTEPS;
    s.step_delay           = TMC2209_DEFAULT_STEP_DELAY;
    s.steps                = 0;
    s.step                 = 0;

    *stepper = s;
}

void TMC2209_PREFIX(set_steps_per_revolution)(tmc2209_t *stepper, uint16_t steps_per_revolution)
{
    if (stepper == NULL) return;

    stepper->steps_per_revolution = steps_per_revolution;
}

void TMC2209_PREFIX(set_microsteps)(tmc2209_t *stepper, uint8_t microsteps)
{
    if (stepper == NULL) return;

    stepper->microsteps = microsteps;
}

void TMC2209_PREFIX(set_step_delay)(tmc2209_t *stepper, uint32_t microseconds)
{
    if (stepper == NULL) return;

    stepper->step_delay = microseconds;
}

void TMC2209_PREFIX(set_direction)(tmc2209_t *stepper, enum direction_t dir)
{
    if (stepper == NULL) return;

    pinMode(stepper->dir_pin, dir);
}

void TMC2209_PREFIX(step)(tmc2209_t *stepper, uint32_t steps, enum direction_t dir)
{
    if (stepper == NULL) return;

    digitalWrite(stepper->dir_pin, dir);
    stepper->step  = 0;
    stepper->steps = steps;
}

void TMC2209_PREFIX(rotate)(tmc2209_t *stepper, int32_t degree)
{
    if (stepper == NULL) return;

    uint32_t steps = abs(degree) * (stepper->steps_per_revolution * stepper->microsteps) / 360.f;
    enum direction_t dir = degree > 0 ? TMC2209_PREFIX(CW) : TMC2209_PREFIX(CCW);

    TMC2209_PREFIX(step)(stepper, steps, dir);
}

void TMC2209_PREFIX(update)(tmc2209_t *stepper)
{
    if (stepper == NULL) return;

    if (stepper->step < stepper->steps) {
        stepper->step++;

        digitalWrite(stepper->step_pin, HIGH);
        delayMicroseconds(stepper->step_delay);
        digitalWrite(stepper->step_pin, LOW);
        delayMicroseconds(stepper->step_delay);
    }
}