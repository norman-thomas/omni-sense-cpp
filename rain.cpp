#include <Arduino.h>
#include "rain.h"

namespace rain {
  int measure(int pin) {
    int value = analogRead(pin);
    return value;
  }
}

