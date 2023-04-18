#include "Arduino.h"

#ifndef ENCHESS_BLE_H
#define ENCHESS_BLE_H

extern struct SettingsOptions {
  int moveTime;
  bool color;
} settingsOptions;

extern bool deviceConnected;
void setup_BLE(void);
void sendArray(uint8_t *squares, uint8_t size);

#endif // ENCHESS_BLE_H