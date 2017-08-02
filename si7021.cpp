#include "si7021.h"
#include "utils.h"

#include <SparkFun_Si7021_Breakout_Library.h>

namespace si7021 {
  static Weather sensor;

  bool setup() {
    bool status = sensor.begin();
    if (!status) {
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }
    return status;
  }

  Measurement measure() {
    Measurement measurement;
    measurement.humidity = utils::roundReading(sensor.getRH());
    measurement.temperature = utils::roundReading(sensor.getTemp());
    return measurement;
  }
}

