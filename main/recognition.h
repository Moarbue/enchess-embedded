#include "Arduino.h"

#ifndef ENCHESS_RECOGNITION_H
#define ENCHESS_RECOGNITION_H

#define OCCUPIED 0
#define EMPTY    1

extern bool squares[64];

void setup_sensors(void);

#endif // ENCHESS_RECOGNITION_H