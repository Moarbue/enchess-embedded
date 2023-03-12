#include "Arduino.h"

#ifndef ENCHESS_TMC2209_H
#define ENCHESS_TMC2209_H

#ifdef __cplusplus
extern "C"{
#endif


#ifndef TMC2209_PREFIX
#define TMC2209_PREFIX(name) tmc2209_##name
#endif // TMC2209_PREFIX

#ifndef TMC2209_DEFAULT_STEPS_PER_REVOLUTION
#define TMC2209_DEFAULT_STEPS_PER_REVOLUTION 200
#endif // TMC2209_DEFAULT_STEPS_PER_REVOLUTION

#ifndef TMC2209_DEFAULT_MICROSTEPS
#define TMC2209_DEFAULT_MICROSTEPS 8
#endif // TMC2209_DEFAULT_MICROSTEPS

#ifndef TMC2209_DEFAULT_STEP_DELAY
#define TMC2209_DEFAULT_STEP_DELAY 1000
#endif // TMC2209_DEFAULT_STEP_DELAY

typedef struct {
    uint8_t dir_pin;
    uint8_t step_pin;
    uint8_t en_pin;

    uint16_t steps_per_revolution;
    uint8_t  microsteps;
    uint32_t step_delay;

    uint32_t step;
    uint32_t steps;
} tmc2209_t;

enum direction_t { TMC2209_PREFIX(CCW), TMC2209_PREFIX(CW) };

void TMC2209_PREFIX(new)(tmc2209_t *stepper, uint8_t dir_pin, uint8_t step_pin, uint8_t en_pin);

void TMC2209_PREFIX(set_steps_per_revolution)(tmc2209_t *stepper, uint16_t steps_per_revolution);

void TMC2209_PREFIX(set_microsteps)(tmc2209_t *stepper, uint8_t microsteps);

void TMC2209_PREFIX(set_step_delay)(tmc2209_t *stepper, uint32_t microseconds);

void TMC2209_PREFIX(set_direction)(tmc2209_t *stepper, enum direction_t dir);

void TMC2209_PREFIX(step)(tmc2209_t *stepper, uint32_t steps, enum direction_t dir);

void TMC2209_PREFIX(rotate)(tmc2209_t *stepper, int32_t degree);

void TMC2209_PREFIX(update)(tmc2209_t *stepper);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // ENCHESS_TMC2209_H