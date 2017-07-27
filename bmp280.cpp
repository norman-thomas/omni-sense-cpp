#include "bmp280.h"
#include "utils.h"

namespace bmp280 {
  bool setup() {
    bool status = bmp.begin(0x76);
    if (!status) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }
    return status;
  }

  Measurement measure() {
    Measurement measurement;
    measurement.temperature = utils::roundReading(bmp.readTemperature());
    measurement.pressure = utils::roundReading(bmp.readPressure() / 100.0);
    return measurement;
  }
}

