#ifndef OMNI_PPD42NS_H_
#define OMNI_PPD42NS_H_

namespace ppd42ns {
  struct Measurement {
      double ratio;
      double concentration;
  };

  Measurement measure(int pin, int duration);
}

#endif
