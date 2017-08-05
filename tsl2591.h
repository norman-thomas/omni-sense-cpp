#ifndef OMNI_TSL2591_H_
#define OMNI_TSL2591_H_

#include <Arduino.h>
#include <stdint.h>
#include <map>

namespace tsl2591 {
  struct Measurement {
    uint16_t lux;
    uint16_t ir;
    uint16_t visible;
    uint16_t full;
  };

  bool setup();
  Measurement measure(std::map<String, String> &environment);
}

#endif
