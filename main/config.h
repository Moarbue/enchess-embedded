#ifndef ENCHESS_CONFIG_H
#define ENCHESS_CONFIG_H

#define ENCHESS_VERSION_MAJOR 1
#define ENCHESS_VERSION_MINOR 0

#define ENCHESS_BLE_NAME               "ENCHESS"
#define ENCHESS_SERVICE_UUID           "bec6075b-97fc-4f3b-a693-f98ace1b5913" // UART service UUID
#define ENCHESS_CHARACTERISTIC_UUID_RX "fb3a5803-d9f9-46b7-b418-9423a7320c08"
#define ENCHESS_CHARACTERISTIC_UUID_TX "ec344566-b4c4-48ca-ad36-a3317b42fbde"

// hardware specific configuration
#define ENCHESS_BOARD_SIZE           240 // board size in mm
#define ENCHESS_THREADED_ROD_LEAD    8.0 // lead of threaded rod in mm
#define ENCHESS_STEPS_PER_REVOLUTION 200 // motor steps for one full turn (1.8° per step)
#define ENCHESS_BOARD_OFFSET_X       0   // Offset in mm to the bottom left corner of the board
#define ENCHESS_BOARD_OFFSET_Y       0   // Offset in mm to the bottom left corner of the board

// general configuration
#define ENCHESS_RPM                  60                     // rotations per minute
#define ENCHESS_MICROSTEPS           TMC2209_MICROSTEPS_4   // divider for each step
#define ENCHESS_STALLGUARD_THRS_COL  30                     // sensibility of stall detection for the column motors 0...255
#define ENCHESS_STALLGUARD_THRS_ROW  20                     // sensibility of stall detection for the row    motor  0...255

// homing related configuration
#define ENCHESS_HOME_RPM                  24   // rotations per minute during homing
#define ENCHESS_HOME_RETRACTION           5    // retraction distance in mm after stall is detected
#define ENCHESS_HOME_STALLGUARD_THRS_COL  5    // sensibility of stall detection for the column motors during homing 0...255
#define ENCHESS_HOME_STALLGUARD_THRS_ROW  2    // sensibility of stall detection for the row    motor during homing  0...255

#endif // ENCHESS_CONFIG_H