#ifndef OMNI_SI7021_H_
#define OMNI_SI7021_H_

namespace si7021 {
  struct Measurement {
    double temperature;
    double humidity;
  };
  
  bool setup();
  Measurement measure();
}

#endif
