#include "Arduino.h"

#include "enchess_pinout.h"
#include "util.h"
#include "config.h"
#include "movement.h"
#include "BLE.h"
#include "recognition.h"

void setup() {
  pinMode(ENCHESS_PIN_LED1, OUTPUT);
  pinMode(ENCHESS_PIN_LED2, OUTPUT);
  pinMode(ENCHESS_PIN_LED3, OUTPUT);

  LOG_MSG("Welcome to Enchess %d.%d!", ENCHESS_VERSION_MAJOR, ENCHESS_VERSION_MINOR);
  
  setup_BLE();
  setup_motors();
  setup_sensors();

  home_motors();
}

void loop() {
  delay(1);
}