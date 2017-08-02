#ifndef OMNI_BMP280_H_
#define OMNI_BMP280_H_

namespace bmp280 {
  struct Measurement {
    double temperature;
    double pressure;
  };
  
  bool setup();
  Measurement measure();
}

#endif
