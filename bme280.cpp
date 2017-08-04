#include "bme280.h"
#include "utils.h"

#include <Adafruit_BME280.h>

namespace bme280 {
  static Adafruit_BME280 sensor;

  bool setup() {
    bool status = sensor.begin(0x76);
    if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
    else {
      sensor.setSampling(
          Adafruit_BME280::MODE_FORCED,
          Adafruit_BME280::SAMPLING_X1, // temperature
          Adafruit_BME280::SAMPLING_X1, // pressure
          Adafruit_BME280::SAMPLING_X1, // humidity
          Adafruit_BME280::FILTER_OFF
      );
    }
    return status;
  }

  Measurement measure(std::map<String, String> &environment, float sealevel) {
    sensor.takeForcedMeasurement();

    Measurement measurement;
    measurement.temperature = utils::roundReading(sensor.readTemperature());
    measurement.humidity = utils::roundReading(sensor.readHumidity());
    measurement.pressure = utils::roundReading(sensor.readPressure() / 100.0);
    measurement.altitude = utils::roundReading(sensor.readAltitude(sealevel));

    environment["temperature"] = String(measurement.temperature);
    environment["humidity"] = String(measurement.humidity);
    environment["pressure"] = String(measurement.pressure);
    environment["altitude"] = String(measurement.altitude);

    return measurement;
  }
}

