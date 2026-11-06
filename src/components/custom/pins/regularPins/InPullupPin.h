#include "../InputPinBase.h"

class InPullupPin : public InputPinBase {
  uint8_t pin;

public:
  explicit InPullupPin(uint8_t pin) : pin(pin) {}
  void begin() override { pinMode(pin, INPUT_PULLUP); }
  int readAnalog() override { return analogRead(pin); }
  int readDigital() override { return digitalRead(pin); }
  uint8_t getPin() override { return pin; }
};
