#include "recognition.h"

#include "config.h"
#include "enchess_pinout.h"
#include "util.h"
#include "BLE.h"

#define is_bit_set(value, bit) (((value) & (1 << bit)) ? HIGH : LOW)

#define SQUARES_COUNT 64

#define swap(x,y) do \ 
   { unsigned char swap_temp[sizeof(x) == sizeof(y) ? (signed)sizeof(x) : -1];  \
     memcpy(swap_temp,&y,sizeof(x));                                            \
     memcpy(&y,&x,       sizeof(x));                                            \
     memcpy(&x,swap_temp,sizeof(x));                                            \
    } while(0)

typedef enum {
    white_pawn = 0,
    white_king,
    white_queen,
    white_bishop,
    white_rook,
    white_knight,

    black_pawn,
    black_king,
    black_queen,
    black_bishop,
    black_rook,
    black_knight,

    none,
} Pieces;

Pieces squares[SQUARES_COUNT] = {
    black_rook, black_knight, black_bishop, black_queen, black_king, black_bishop, black_knight, black_rook,
    black_pawn, black_pawn,   black_pawn,   black_pawn,  black_pawn, black_pawn,   black_pawn,   black_pawn,
    none,       none,         none,         none,        none,       none,         none,         none,
    none,       none,         none,         none,        none,       none,         none,         none,
    none,       none,         none,         none,        none,       none,         none,         none,
    none,       none,         none,         none,        none,       none,         none,         none,
    white_rook, white_knight, white_bishop, white_queen, white_king, white_bishop, white_knight, white_rook,
    white_pawn, white_pawn,   white_pawn,   white_pawn,  white_pawn, white_pawn,   white_pawn,   white_pawn,
};

void query_task(void *param);

void setup_sensors(void)
{
    assert(ENCHESS_SENSOR_QUERY_INTERVAL >= SQUARES_COUNT && "Query interval is to high!");

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
    bool query_results[SQUARES_COUNT] = {0};
    bool old_query_results[SQUARES_COUNT] = {0};

    while(1) {
        digitalWrite(ENCHESS_PIN_TMC_0, is_bit_set(tmc_index, 0));
        digitalWrite(ENCHESS_PIN_TMC_1, is_bit_set(tmc_index, 1));
        digitalWrite(ENCHESS_PIN_TMC_2, is_bit_set(tmc_index, 2));
        digitalWrite(ENCHESS_PIN_TMC_3, is_bit_set(tmc_index, 3));
        digitalWrite(ENCHESS_PIN_TMC_4, is_bit_set(tmc_index, 4));
        digitalWrite(ENCHESS_PIN_TMC_5, is_bit_set(tmc_index, 5));

        delay(1);
        query_results[tmc_index] = digitalRead(ENCHESS_PIN_TMC_OUT);

        if (tmc_index < 63) tmc_index++;
        else {
            tmc_index = 0;
            
            // check if a chess piece was moved
            if (memcmp(query_results, old_query_results, SQUARES_COUNT * sizeof (query_results[0])) == 0) continue;

            // find old and new pos
            // NOTE: it is expected that only one chess piece was moved, information about the type is lost
            uint8_t old_pos, new_pos, j = 0;
            for(uint8_t i = 0; i < SQUARES_COUNT; i++) {
               if (query_results[i] == old_query_results[i]) continue;
               if (query_results[i] == OCCUPIED) new_pos = i;
               if (query_results[i] == EMPTY   ) old_pos = i;

               j++;
               if (j > 2) LOG_MSG("ERROR: More than one chess piece was moved since last query!");
            }

            swap(squares[old_pos], squares[new_pos]);
            sendArray((uint8_t *)squares, SQUARES_COUNT);

            memcpy(old_query_results, query_results, SQUARES_COUNT * sizeof (query_results[0]));
            delay(ENCHESS_SENSOR_QUERY_INTERVAL - SQUARES_COUNT);
        }
    }
}