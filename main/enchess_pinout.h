#ifndef ENCHESS_PINOUT_H
#define ENCHESS_PINOUT_H

/* Pinout of the ESP32-S3 on the Enchess-Control-PCB */

/* 
S1_*  = Stepper Driver X-Axis
S2_*  = Stepper Driver Y-Axis
S_*   = Both Stepper Drivers
TMC_* = Connector to Enchess-Sensor-PCB
*/

#define ENCHESS_PIN_BOOT       0
#define ENCHESS_PIN_TMC_OUT    1
#define ENCHESS_PIN_TMC_0      2
#define ENCHESS_PIN_S1_DIR     3
#define ENCHESS_PIN_LED1       4
#define ENCHESS_PIN_LED2       5
#define ENCHESS_PIN_LED3       6
#define ENCHESS_PIN_S_CLK      9
#define ENCHESS_PIN_S1_MS2     11
#define ENCHESS_PIN_S1_MS1     12
#define ENCHESS_PIN_S1_EN      13
#define ENCHESS_PIN_S2_DIR     14
#define ENCHESS_PIN_EL_MAG     15
#define ENCHESS_PIN_S_TX       17
#define ENCHESS_PIN_S_RX       18
#define ENCHESS_PIN_USB_DMINUS 19
#define ENCHESS_PIN_USB_DPLUS  20
#define ENCHESS_PIN_S2_STEP    21
#define ENCHESS_PIN_TMC_5      38
#define ENCHESS_PIN_TMC_4      39
#define ENCHESS_PIN_TMC_3      40
#define ENCHESS_PIN_TMC_2      41
#define ENCHESS_PIN_TMC_1      42
#define ENCHESS_PIN_S2_EN      45
#define ENCHESS_PIN_S1_STEP    46
#define ENCHESS_PIN_S2_MS2     47
#define ENCHESS_PIN_S2_MS1     48

#endif // ENCHESS_PINOUT_H