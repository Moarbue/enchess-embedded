#include "recognition.h"

#include "config.h"
#include "enchess_pinout.h"
#include "util.h"
#include "BLE.h"

#define is_bit_set(value, bit) (((value) & (1 << bit)) ? HIGH : LOW)

#define SENSOR_INTERVAL 10

#define swap(x,y) do                                                            \
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1];  \
     memcpy(swap_temp,&y,sizeof(x));                                            \
     memcpy(&y,&x,       sizeof(x));                                            \
     memcpy(&x,swap_temp,sizeof(x));                                            \
    } while(0)

Pieces squares[SQUARES_COUNT] = {
    black_rook, black_knight, black_bishop, black_queen, black_king, black_bishop, black_knight, black_rook,
    black_pawn, black_pawn,   black_pawn,   black_pawn,  black_pawn, black_pawn,   black_pawn,   black_pawn,
    none,       none,         none,         none,        none,       none,         none,         none,
    none,       none,         none,         none,        none,       none,         none,         none,
    none,       none,         none,         none,        none,       none,         none,         none,
    none,       none,         none,         none,        none,       none,         none,         none,
    white_pawn, white_pawn,   white_pawn,   white_pawn,  white_pawn, white_pawn,   white_pawn,   white_pawn,
    white_rook, white_knight, white_bishop, white_queen, white_king, white_bishop, white_knight, white_rook,
};

void query_task(void *param);

void setup_sensors(void)
{
    assert(ENCHESS_SENSOR_QUERY_INTERVAL >= SENSOR_INTERVAL * SQUARES_COUNT && "Query interval is to high!");

    LOG_MSG("INFO: Setting up sensors...");
    pinMode(ENCHESS_PIN_TMC_0, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_1, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_2, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_3, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_4, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_5, OUTPUT);
    pinMode(ENCHESS_PIN_TMC_OUT, INPUT);

    TaskHandle_t query_task_handle;
    xTaskCreatePinnedToCore(query_task, "Query Task", 10000, NULL, 999, &query_task_handle, 0);
}

void query_sensors(bool *sensors, uint8_t size)
{
    for (uint8_t i = 0; i < size; i++) {
        digitalWrite(ENCHESS_PIN_TMC_0, is_bit_set(i, 0));
        digitalWrite(ENCHESS_PIN_TMC_1, is_bit_set(i, 1));
        digitalWrite(ENCHESS_PIN_TMC_2, is_bit_set(i, 2));
        digitalWrite(ENCHESS_PIN_TMC_3, is_bit_set(i, 3));
        digitalWrite(ENCHESS_PIN_TMC_4, is_bit_set(i, 4));
        digitalWrite(ENCHESS_PIN_TMC_5, is_bit_set(i, 5));

        uint8_t col = 7 - i / 8;
        uint8_t row = i % 8;
        if (col % 2 == 1) row = 7 - row;

        delay(SENSOR_INTERVAL);
        if (i % 16 == 0) delay(5000);
        sensors[row * 8 + col] = digitalRead(ENCHESS_PIN_TMC_OUT);
    }
}

void query_task(void *param)
{
    bool query_results[SQUARES_COUNT] = {0};
    bool old_query_results[SQUARES_COUNT] = {0};

    query_sensors(query_results, SQUARES_COUNT);
    memcpy(old_query_results, query_results, SQUARES_COUNT * sizeof (query_results[0]));
    for (uint8_t i = 0; i < SQUARES_COUNT; i++) {
        squares[i] = (query_results[i] == OCCUPIED) ? squares[i] : none;
    }
    sendArray(squares, SQUARES_COUNT);

    while(1) {
        delay(ENCHESS_SENSOR_QUERY_INTERVAL - SENSOR_INTERVAL * SQUARES_COUNT);

        if (digitalRead(ENCHESS_PIN_EL_MAG) == HIGH) continue;

        // query sensors
        query_sensors(query_results, SQUARES_COUNT);
        for (uint8_t i = 0; i < SQUARES_COUNT; i++) {
            if (i % 8 == 0) LOG_MSG();
            printf("%u ", query_results[i]);
        }
        LOG_MSG();
        
        // check if pieces moved since last query
        if (memcmp(query_results, old_query_results, SQUARES_COUNT * sizeof (query_results[0])) == 0) continue;

        // find old and new pos
        // NOTE: it is expected that only one chess piece was moved, else information about the type is lost
        uint8_t old_pos = 0, new_pos = 0, j = 0;
        for(uint8_t i = 0; i < SQUARES_COUNT; i++) {
           if (query_results[i] == old_query_results[i]) continue;
           if (query_results[i] == OCCUPIED) new_pos = i;
           if (query_results[i] == EMPTY   ) old_pos = i;

           j++;
        }
        if (j > 2) LOG_MSG("ERROR: More than one chess piece was moved since last query!");

        if (j == 1) continue;

        // update pieces position and notify mobile app
        swap(squares[old_pos], squares[new_pos]);
        sendArray(squares, SQUARES_COUNT);

        // update old values
        memcpy(old_query_results, query_results, SQUARES_COUNT * sizeof (query_results[0]));
    }
}