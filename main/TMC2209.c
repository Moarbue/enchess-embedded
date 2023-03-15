#include "TMC2209.h"

#define NO_PIN (uint8_t)-1

void TMC2209_PREFIX(new)(tmc2209_t *stepper, uint8_t dir_pin, uint8_t step_pin,
                                             uint8_t en_pin,  uint8_t ms1_pin, uint8_t ms2_pin)
{
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(en_pin, OUTPUT);
    pinMode(ms1_pin, OUTPUT);
    pinMode(ms2_pin, OUTPUT);

    digitalWrite(en_pin, LOW);

    tmc2209_t s = {0};
    s.dir_pin              = dir_pin;
    s.step_pin             = step_pin;
    s.en_pin               = en_pin;
    s.ms1_pin              = ms1_pin;
    s.ms2_pin              = ms2_pin;
    s.rx_pin               = NO_PIN;
    s.tx_pin               = NO_PIN;
    s.addr                 = TMC2209_NO_ADRESS;
    s.steps_per_revolution = TMC2209_DEFAULT_STEPS_PER_REVOLUTION;
    s.microsteps           = TMC2209_DEFAULT_MICROSTEPS;
    s.step_delay           = TMC2209_DEFAULT_STEP_DELAY;
    s.steps                = 0;
    s.step                 = 0;

    *stepper = s;
}

void TMC2209_PREFIX(new_full)(tmc2209_t *stepper, uint8_t dir_pin, uint8_t step_pin, uint8_t en_pin,
                              uint8_t rx_pin, uint8_t tx_pin, uint8_t ms1_pin, uint8_t ms2_pin, tmc2209_adress addr)
{
    pinMode(step_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
    pinMode(en_pin, OUTPUT);
    pinMode(ms1_pin, OUTPUT);
    pinMode(ms2_pin, OUTPUT);

    digitalWrite(en_pin, LOW);

    tmc2209_t s = {0};
    s.dir_pin              = dir_pin;
    s.step_pin             = step_pin;
    s.en_pin               = en_pin;
    s.ms1_pin              = ms1_pin;
    s.ms2_pin              = ms2_pin;
    s.rx_pin               = rx_pin;
    s.tx_pin               = tx_pin;
    s.addr                 = addr;
    s.steps_per_revolution = TMC2209_DEFAULT_STEPS_PER_REVOLUTION;
    s.microsteps           = TMC2209_DEFAULT_MICROSTEPS;
    s.step_delay           = TMC2209_DEFAULT_STEP_DELAY;
    s.steps                = 0;
    s.step                 = 0;

    *stepper = s;
}

void TMC2209_PREFIX(set_adress)(tmc2209_t *stepper, tmc2209_adress addr)
{
    switch (addr) {
        case TMC2209_ADRESS_0:
            digitalWrite(stepper->ms1_pin, LOW);
            digitalWrite(stepper->ms2_pin, LOW);
        break;
        case TMC2209_ADRESS_1:
            digitalWrite(stepper->ms1_pin, HIGH);
            digitalWrite(stepper->ms2_pin, LOW);
        break;
        case TMC2209_ADRESS_2:
            digitalWrite(stepper->ms1_pin, LOW);
            digitalWrite(stepper->ms2_pin, HIGH);
        break;
        case TMC2209_ADRESS_3:
            digitalWrite(stepper->ms1_pin, HIGH);
            digitalWrite(stepper->ms2_pin, HIGH);
        break;
        default:
            // TODO: Implement Error Reporting
            assert(false && "Wrong Adress specified!");
    }
}

void TMC2209_PREFIX(set_steps_per_revolution)(tmc2209_t *stepper, uint16_t steps_per_revolution)
{
    if (stepper == NULL) return;

    stepper->steps_per_revolution = steps_per_revolution;
}

void TMC2209_PREFIX(set_microsteps)(tmc2209_t *stepper, tmc2209_microstep microsteps)
{
    if (stepper == NULL) return;

    // TODO: Implement Error Reporting
    if (microsteps != TMC2209_MICROSTEPS_8  || microsteps != TMC2209_MICROSTEPS_16 ||
        microsteps != TMC2209_MICROSTEPS_32 || microsteps != TMC2209_MICROSTEPS_64)
        return;

    stepper->microsteps = microsteps;

    if (stepper->rx_pin == NO_PIN || stepper->tx_pin == NO_PIN ) {
        switch (microsteps) {
            case TMC2209_MICROSTEPS_8:
                digitalWrite(stepper->ms1_pin, LOW);
                digitalWrite(stepper->ms2_pin, LOW);
            break;
            case TMC2209_MICROSTEPS_16:
                digitalWrite(stepper->ms1_pin, HIGH);
                digitalWrite(stepper->ms2_pin, HIGH);
            break;
            case TMC2209_MICROSTEPS_32:
                digitalWrite(stepper->ms1_pin, HIGH);
                digitalWrite(stepper->ms2_pin, LOW);
            break;
            case TMC2209_MICROSTEPS_64:
                digitalWrite(stepper->ms1_pin, LOW);
                digitalWrite(stepper->ms2_pin, HIGH);
            break;
        } 
    } else {
        // TODO: Implement setting microsteps via UART
        assert(false && "Unimplemented: set_microsteps via UART");
    }   
}

void TMC2209_PREFIX(set_step_delay)(tmc2209_t *stepper, uint32_t microseconds)
{
    if (stepper == NULL) return;

    stepper->step_delay = microseconds;
}

void TMC2209_PREFIX(set_direction)(tmc2209_t *stepper, enum direction_t dir)
{
    if (stepper == NULL) return;

    // TODO: Implement setting direction via UART?
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