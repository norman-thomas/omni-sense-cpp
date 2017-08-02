#ifndef OMNI_TSL2591_H_
#define OMNI_TSL2591_H_

#include <stdint.h>

namespace tsl2591 {
  struct Measurement {
    uint16_t lux;
    uint16_t ir;
    uint16_t visible;
  };

  bool setup();
  Measurement measure();
}

#endif
