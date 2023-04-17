#include "recognition.h"

#include "config.h"
#include "enchess_pinout.h"
#include "util.h"

#define is_bit_set(value, bit) (((value) & (1 << bit)) ? HIGH : LOW)

bool squares[64] = {0};

void query_task(void *param);

void setup_sensors(void)
{
    assert(ENCHESS_SENSOR_QUERY_INTERVAL >= 64 && "Query interval is to high!");

    LOG_MSG("INFO: Setting up sensors...");
    pinMode(ENCHESS_PIN_TMC_0, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_1, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_2, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_3, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_4, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_5, OUTPUT);

    TaskHandle_t query_task_handle;
    xTaskCreatePinnedToCore(query_task, "Query Task", 10000, NULL, 999, &query_task_handle, 0);
}

void query_task(void *param)
{
    uint8_t tmc_index = 0;
    while(1) {
        digitalWrite(ENCHESS_PIN_TMC_0, is_bit_set(tmc_index, 0));
        digitalWrite(ENCHESS_PIN_TMC_1, is_bit_set(tmc_index, 1));
        digitalWrite(ENCHESS_PIN_TMC_2, is_bit_set(tmc_index, 2));
        digitalWrite(ENCHESS_PIN_TMC_3, is_bit_set(tmc_index, 3));
        digitalWrite(ENCHESS_PIN_TMC_4, is_bit_set(tmc_index, 4));
        digitalWrite(ENCHESS_PIN_TMC_5, is_bit_set(tmc_index, 5));

        delay(1);
        squares[tmc_index] = digitalRead(ENCHESS_PIN_TMC_OUT);

        if (tmc_index < 63) tmc_index++;
        else {
            tmc_index = 0;
            delay(ENCHESS_SENSOR_QUERY_INTERVAL - 64);
        }
    }
}