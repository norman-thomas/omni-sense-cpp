#include "tsl2561.h"

#include <TSL2561.h>

#include <vector>
#include <utility>

namespace tsl2561 {
  TSL2561 sensor = TSL2561(TSL2561_ADDR_FLOAT);
  unsigned long MAX_LUX = 40000;
  unsigned long MAX_VALUE = 65535;

  bool setup() {
    if(!sensor.begin())
    {
      Serial.println("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
      return false;
    }

    sensor.setGain(TSL2561_GAIN_16X);
    sensor.setTiming(TSL2561_INTEGRATIONTIME_402MS);
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

    std::vector< std::pair<tsl2561Gain_t, tsl2561IntegrationTime_t> > configs = {
      {TSL2561_GAIN_16X, TSL2561_INTEGRATIONTIME_402MS},
      {TSL2561_GAIN_0X, TSL2561_INTEGRATIONTIME_402MS},
      {TSL2561_GAIN_16X, TSL2561_INTEGRATIONTIME_101MS},
      {TSL2561_GAIN_0X, TSL2561_INTEGRATIONTIME_101MS},
      {TSL2561_GAIN_16X, TSL2561_INTEGRATIONTIME_13MS},
      {TSL2561_GAIN_0X, TSL2561_INTEGRATIONTIME_13MS}
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
    return m;
  }
}

