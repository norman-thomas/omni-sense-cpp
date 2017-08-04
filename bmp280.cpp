#include "bmp280.h"
#include "utils.h"

#include <Adafruit_BMP280.h>

namespace bmp280 {
  static Adafruit_BMP280 sensor;

  bool setup() {
    bool status = sensor.begin(0x76);
    if (!status) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }
    return status;
  }

  Measurement measure(std::map<String, String> &environment) {
    Measurement measurement;
    measurement.temperature = utils::roundReading(sensor.readTemperature());
    measurement.pressure = utils::roundReading(sensor.readPressure() / 100.0);

    environment["temperature"] = String(measurement.temperature);
    environment["pressure"] = String(measurement.pressure);
    
    return measurement;
  }
}

