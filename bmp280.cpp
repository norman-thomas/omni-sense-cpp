#include "bmp280.h"
#include "utils.h"

namespace bmp280 {
  static Adafruit_BMP280 sensor;

  bool setup() {
    bool status = sensor.begin(0x76);
    if (!status) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }
    return status;
  }

  Measurement measure() {
    Measurement measurement;
    measurement.temperature = utils::roundReading(sensor.readTemperature());
    measurement.pressure = utils::roundReading(sensor.readPressure() / 100.0);
    return measurement;
  }
}

