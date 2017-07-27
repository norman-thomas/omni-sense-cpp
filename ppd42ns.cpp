#include <Arduino.h>
#include "ppd42ns.h"

namespace ppd42ns {
  Measurement measure(int pin, int duration) {
    const int starttime = millis();
    int lowpulseoccupancy = 0;
    while ((millis()-starttime) < duration) {
        int duration = pulseIn(pin, LOW);
        lowpulseoccupancy += duration;
    }

    Measurement m;
    m.ratio = lowpulseoccupancy / (duration * 10.0);
    m.concentration = 1.1 * pow(m.ratio, 3) - 3.8 * pow(m.ratio, 2) + 520 * m.ratio + 0.62;
    return m;
  }
}

