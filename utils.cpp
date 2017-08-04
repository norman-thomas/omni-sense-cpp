#include "utils.h"

#include <math.h>

namespace utils {
  double roundReading(double reading) {
      return (floor(reading * 100.0)) / 100.0;
  }

  String toJSON(std::map<String, String> env) {
    String result = "";

    for (auto const &entry : env) {
      result += " \"" + entry.first + "\": " + entry.second + ",";
    }

    if (!env.empty()) {
      result.remove(result.length() - 1);
      result = "{" + result + "}";
    }
    return result;
  }
}

