#ifndef OMNI_BME280_H_
#define OMNI_BME280_H_

#include <Adafruit_BME280.h>

namespace bme280 {
  struct Measurement {
    double temperature;
    double humidity;
    double pressure;
    double altitude;
  };
  
  bool setup();
  Measurement measure(float sealevel = 1013.25f);
}

#endif
