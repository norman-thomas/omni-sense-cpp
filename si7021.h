#ifndef OMNI_SI7021_H_
#define OMNI_SI7021_H_

#include <Arduino.h>
#include <map>

namespace si7021 {
  struct Measurement {
    double temperature;
    double humidity;
  };
  
  bool setup();
  Measurement measure(std::map<String, String> &environment);
}

#endif
