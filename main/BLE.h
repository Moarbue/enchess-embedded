#include "Arduino.h"

#ifndef ENCHESS_BLE_H
#define ENCHESS_BLE_H

extern struct SettingsOptions {
  int moveTime;
  bool color;
} settingsOptions;

extern bool deviceConnected;
void setup_BLE(void);

#endif // ENCHESS_BLE_H