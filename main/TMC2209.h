#include "Arduino.h"

#ifndef ENCHESS_TMC2209_H
#define ENCHESS_TMC2209_H

#ifdef __cplusplus
extern "C"{
#endif


#ifndef TMC2209_PREFIX
#define TMC2209_PREFIX(name) tmc2209_##name
#endif // TMC2209_PREFIX

typedef enum {
    TMC2209_ADRESS_0 = 0x0b00,
    TMC2209_ADRESS_1 = 0x0b01,
    TMC2209_ADRESS_2 = 0x0b10,
    TMC2209_ADRESS_3 = 0x0b11,
    TMC2209_NO_ADRESS = 0xFFFF,
} tmc2209_adress;

typedef enum {
    TMC2209_MICROSTEPS_8  = 8,
    TMC2209_MICROSTEPS_16 = 16,
    TMC2209_MICROSTEPS_32 = 32,
    TMC2209_MICROSTEPS_64 = 64,
} tmc2209_microstep;

#ifndef TMC2209_DEFAULT_STEPS_PER_REVOLUTION
#define TMC2209_DEFAULT_STEPS_PER_REVOLUTION 200
#endif // TMC2209_DEFAULT_STEPS_PER_REVOLUTION

#ifndef TMC2209_DEFAULT_MICROSTEPS
#define TMC2209_DEFAULT_MICROSTEPS TMC2209_MICROSTEPS_8
#endif // TMC2209_DEFAULT_MICROSTEPS

#ifndef TMC2209_DEFAULT_STEP_DELAY
#define TMC2209_DEFAULT_STEP_DELAY 1000
#endif // TMC2209_DEFAULT_STEP_DELAY

typedef struct {
    uint8_t dir_pin;
    uint8_t step_pin;
    uint8_t en_pin;
    uint8_t ms1_pin;
    uint8_t ms2_pin;
    uint8_t rx_pin;
    uint8_t tx_pin;
    tmc2209_adress addr;

    uint16_t steps_per_revolution;
    tmc2209_microstep microsteps;
    uint32_t step_delay;

    uint32_t step;
    uint32_t steps;
} tmc2209_t;

enum direction_t { TMC2209_PREFIX(CCW), TMC2209_PREFIX(CW) };

void TMC2209_PREFIX(new)(tmc2209_t *stepper, uint8_t dir_pin, uint8_t step_pin,
                                             uint8_t en_pin,  uint8_t ms1_pin, uint8_t ms2_pin);

void TMC2209_PREFIX(new_full)(tmc2209_t *stepper, uint8_t dir_pin, uint8_t step_pin, uint8_t en_pin,
                              uint8_t rx_pin, uint8_t tx_pin, uint8_t ms1_pin, uint8_t ms2_pin, tmc2209_adress addr);

void TMC2209_PREFIX(set_adress)(tmc2209_t *stepper, tmc2209_adress addr);

void TMC2209_PREFIX(set_steps_per_revolution)(tmc2209_t *stepper, uint16_t steps_per_revolution);

void TMC2209_PREFIX(set_microsteps)(tmc2209_t *stepper, tmc2209_microstep microsteps);

void TMC2209_PREFIX(set_step_delay)(tmc2209_t *stepper, uint32_t microseconds);

void TMC2209_PREFIX(set_direction)(tmc2209_t *stepper, enum direction_t dir);

void TMC2209_PREFIX(step)(tmc2209_t *stepper, uint32_t steps, enum direction_t dir);

void TMC2209_PREFIX(rotate)(tmc2209_t *stepper, int32_t degree);

void TMC2209_PREFIX(update)(tmc2209_t *stepper);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // ENCHESS_TMC2209_H