#include "tsl2591.h"

#include <Adafruit_TSL2591.h>

namespace tsl2591 {
  Adafruit_TSL2591 sensor = Adafruit_TSL2591(2591);

  bool setup() {
    if(!sensor.begin())
    {
      Serial.println("Ooops, no TSL2591 detected ... Check your wiring or I2C ADDR!");
      return false;
    }

    sensor.setGain(TSL2591_GAIN_MED);
    sensor.setTiming(TSL2591_INTEGRATIONTIME_200MS);
    return true;
  }

  Measurement measure() {
    Measurement m;

    uint32_t lum = sensor.getFullLuminosity();
    uint16_t ir, full;
    ir = lum >> 16;
    full = lum & 0xFFFF;
    uint16_t visible = full - ir;
    m.lux = sensor.calculateLux(full, ir);
    m.ir = ir;
    m.visible = visible;
    
    return m;
  }
}

