#ifndef ENCHESS_CONFIG_H
#define ENCHESS_CONFIG_H

#define ENCHESS_VERSION_MAJOR 1
#define ENCHESS_VERSION_MINOR 0

#define ENCHESS_SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define ENCHESS_CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define ENCHESS_CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// hardware specific configuration
#define ENCHESS_BOARD_SIZE           240 // board size in mm
#define ENCHESS_THREADED_ROD_LEAD    8.0 // lead of threaded rod in mm
#define ENCHESS_STEPS_PER_REVOLUTION 200 // motor steps for one full turn (1.8° per step)
#define ENCHESS_BOARD_OFFSET_X       0   // Offset in mm to the bottom left corner of the board
#define ENCHESS_BOARD_OFFSET_Y       0   // Offset in mm to the bottom left corner of the board

// general configuration
#define ENCHESS_STEP_DELAY           300 // delay between steps in µs
#define ENCHESS_MICROSTEPS           8   // divider for each step
#define ENCHESS_STALLGUARD_THRS      100 // sensibility of stall detection 0...255

// homing related configuration
#define ENCHESS_HOME_STEP_DELAY      300 // delay between steps in µs during homing
#define ENCHESS_HOME_RETRACTION      5   // retraction distance in mm after stall is detected

#endif // ENCHESS_CONFIG_H