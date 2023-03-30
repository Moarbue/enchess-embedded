#ifndef ENCHESS_CONFIG_H
#define ENCHESS_CONFIG_H

#define ENCHESS_VERSION_MAJOR 1
#define ENCHESS_VERSION_MINOR 0

#define ENCHESS_SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define ENCHESS_CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define ENCHESS_CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

#define ENCHESS_BOARD_SIZE        240 // board size in mm
#define ENCHESS_THREADED_ROD_LEAD 8.0 // lead of threaded rod in mm

#define ENCHESS_STEP_DELAY           300 // delay between steps in µs
#define ENCHESS_HOME_STEP_DELAY_1    300 // delay between steps in µs during first homing
#define ENCHESS_HOME_STEP_DELAY_2    800 // delay between steps in µs during second homing
#define ENCHESS_HOME_RETRACTION      5   // retraction distance in mm after stall is detected
#define ENCHESS_STEPS_PER_REVOLUTION 200 // motor steps for one full turn (1.8° per step)
#define ENCHESS_MICROSTEPS           8   // divider for each step

#endif // ENCHESS_CONFIG_H