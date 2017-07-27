#include <Adafruit_BMP280.h>

namespace bmp280 {
  static Adafruit_BMP280 bmp;
  struct Measurement {
    double temperature;
    double pressure;
  };
  
  bool setup();
  Measurement measure();
}

