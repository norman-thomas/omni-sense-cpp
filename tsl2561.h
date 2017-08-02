#ifndef OMNI_TSL2561_H_
#define OMNI_TSL2561_H_

#include <stdint.h>

namespace tsl2561 {
  struct Measurement {
    uint16_t lux;
    uint16_t visible;
    uint16_t ir;
  };

  bool setup();
  Measurement measure();
}

#endif
