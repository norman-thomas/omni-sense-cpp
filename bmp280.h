#ifndef OMNI_BMP280_H_
#define OMNI_BMP280_H_

#include <Arduino.h>
#include <map>

namespace bmp280 {
  struct Measurement {
    double temperature;
    double pressure;
  };
  
  bool setup();
  Measurement measure(std::map<String, String> &environment);
}

#endif
