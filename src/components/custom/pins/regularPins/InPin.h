#include "../InputPinBase.h"

class InPin : public InputPinBase {
  uint8_t pin;

public:
  explicit InPin(uint8_t pin) : pin(pin) {}
  void begin() override { pinMode(pin, INPUT); }
  int readAnalog() override { return analogRead(pin); }
  int readDigital() override { return digitalRead(pin); }
  uint8_t getPin() override { return pin; }
};
