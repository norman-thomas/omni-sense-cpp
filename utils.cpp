#include "utils.h"

#include <math.h>

namespace utils {
  double roundReading(double reading) {
      return (floor(reading * 100.0)) / 100.0;
  }
}

