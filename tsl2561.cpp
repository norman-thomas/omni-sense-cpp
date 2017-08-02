#include "tsl2561.h"

#include <Adafruit_TSL2561_U.h>

namespace tsl2561 {
  Adafruit_TSL2561_Unified sensor = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

  bool setup() {
    if(!sensor.begin())
    {
      Serial.println("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
      return false;
    }

    sensor.enableAutoRange(true);
    sensor.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);
    return true;
  }

  Measurement measure() {
    Measurement m;
    
    sensors_event_t event;
    sensor.getEvent(&event);

    if (event.light) {
      m.lux = event.light;
      m.visible = 0;
      m.ir = 0;
    }
    
    return m;
  }
}

