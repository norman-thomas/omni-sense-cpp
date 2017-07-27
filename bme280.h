#include <Adafruit_BME280.h>

#include <set>
#include <map>

namespace bme280 {
  static Adafruit_BME280 bme;
  struct Measurement {
    double temperature;
    double humidity;
    double pressure;
    double altitude;
  };
  
  bool setup();
  Measurement measure(float sealevel = 1013.25f);
}

