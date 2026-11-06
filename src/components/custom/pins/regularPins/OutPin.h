#include "../OutputPinBase.h"

class OutPin : public OutputPinBase {
  uint8_t pin;

public:
  explicit OutPin(uint8_t pin) : pin(pin) {}
  void begin() override { pinMode(pin, OUTPUT); }
  void writeAnalog(int val) override { analogWrite(pin, val); }
  void writeDigital(int val) override { digitalWrite(pin, val); }
  uint8_t getPin() override { return pin; }
};
