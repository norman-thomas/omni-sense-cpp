#include "bmp280.h"

namespace bmp280 {
  bool setup() {
    bool status = bmp.begin(0x76);
    if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
    return status;
  }
}

