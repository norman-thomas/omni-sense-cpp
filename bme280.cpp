#include "bme280.h"
#include "utils.h"

namespace bme280 {
  bool setup() {
    bool status = bme.begin(0x76);
    if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
    else {
      bme.setSampling(
          Adafruit_BME280::MODE_FORCED,
          Adafruit_BME280::SAMPLING_X1, // temperature
          Adafruit_BME280::SAMPLING_X1, // pressure
          Adafruit_BME280::SAMPLING_X1, // humidity
          Adafruit_BME280::FILTER_OFF
      );
    }
    return status;
  }

  Measurement measure(float sealevel) {
    bme.takeForcedMeasurement();

    Measurement measurement;
    measurement.temperature = utils::roundReading(bme.readTemperature());
    measurement.humidity = utils::roundReading(bme.readHumidity());
    measurement.pressure = utils::roundReading(bme.readPressure() / 100.0);
    measurement.altitude = utils::roundReading(bme.readAltitude(sealevel));
    return measurement;
  }
}

