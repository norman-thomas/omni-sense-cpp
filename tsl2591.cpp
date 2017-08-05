#include "tsl2591.h"

#include <Adafruit_TSL2591.h>

#include <vector>
#include <utility>

namespace tsl2591 {
  Adafruit_TSL2591 sensor = Adafruit_TSL2591(2591);
  unsigned long MAX_LUX = 40000;
  unsigned long MAX_VALUE = 65535;

  bool setup() {
    if(!sensor.begin())
    {
      Serial.println("Ooops, no TSL2591 detected ... Check your wiring or I2C ADDR!");
      return false;
    }

    sensor.setGain(TSL2591_GAIN_MAX);
    sensor.setTiming(TSL2591_INTEGRATIONTIME_400MS);
    return true;
  }

  Measurement _measure() {
    Measurement m;
    uint32_t lum = sensor.getFullLuminosity();
    m.ir = lum >> 16;
    m.full = lum & 0xFFFF;
    m.visible = m.full - m.ir;
    m.lux = sensor.calculateLux(m.visible, m.ir);
    return m;
  }

  Measurement measure(std::map<String, String> &environment) {
    Measurement m;

    std::vector< std::pair<tsl2591Gain_t, tsl2591IntegrationTime_t> > configs = {
      {TSL2591_GAIN_MAX, TSL2591_INTEGRATIONTIME_600MS},
      {TSL2591_GAIN_HIGH, TSL2591_INTEGRATIONTIME_600MS},
      {TSL2591_GAIN_HIGH, TSL2591_INTEGRATIONTIME_400MS},
      {TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_400MS},
      {TSL2591_GAIN_MED, TSL2591_INTEGRATIONTIME_200MS},
      {TSL2591_GAIN_LOW, TSL2591_INTEGRATIONTIME_200MS},
      {TSL2591_GAIN_LOW, TSL2591_INTEGRATIONTIME_100MS}
    };

    for (auto const &conf : configs) {
      sensor.setGain(conf.first);
      sensor.setTiming(conf.second);

      m = _measure();
      if (m.lux < MAX_LUX && m.full > MAX_VALUE && m.ir < MAX_VALUE) {
        break;
      }
      Serial.println("Too bright, lowering gain and timing.");
    }
    
    return m;
  }
}

