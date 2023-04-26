#include "Arduino.h"
#include "recognition.h"

#ifndef ENCHESS_BLE_H
#define ENCHESS_BLE_H

extern struct SettingsOptions {
  int moveTime;
  bool color;
} settingsOptions;

extern bool deviceConnected;
void setup_BLE(void);
void sendArray(Pieces *squares, uint8_t size);

#endif // ENCHESS_BLE_H