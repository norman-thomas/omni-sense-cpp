#ifndef OMNI_BMP280_H_
#define OMNI_BMP280_H_

#include <Adafruit_BMP280.h>

namespace bmp280 {
  struct Measurement {
    double temperature;
    double pressure;
  };
  
  bool setup();
  Measurement measure();
}

#endif
