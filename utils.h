#ifndef OMNI_UTILS_H_
#define OMNI_UTILS_H_

#include <Arduino.h>
#include <map>

namespace utils {
  double roundReading(double reading);
  String toJSON(std::map<String, String> env);
}

#endif
