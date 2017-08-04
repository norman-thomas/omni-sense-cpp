#ifndef OMNI_RAIN_H_
#define OMNI_RAIN_H_

#include <Arduino.h>
#include <map>

namespace rain {
  int measure(int pin, std::map<String, String> &environment);
}

#endif
