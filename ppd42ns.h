namespace ppd42ns {
  struct Measurement {
      double ratio;
      double concentration;
  };

  Measurement measure(int pin, int duration);
}

