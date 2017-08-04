#include <Arduino.h>
#include "rain.h"

namespace rain {
  int measure(int pin, std::map<String, String> &environment) {
    int value = analogRead(pin);

    environment["rain"] = String(value);

    return value;
  }
}

