#include "Pin.h"

class UninitializedPin : public Pin {
  uint8_t pin;

public:
  explicit UninitializedPin(uint8_t pin) : pin(pin) {}
  void begin() override {}
  void writeAnalog(int val) override {}
  void writeDigital(int val) override {}
  uint8_t getPin() override { return pin; }
};
