#include "Led.h"

Led::Led(uint8_t pin) : Component<1>({{pin, OUTPUT}}) {}

void Led::setBrightness(uint16_t brightness) {
  analogWrite(getPin(0), brightness);
}

void Led::setBrightness(uint8_t brightness) {
  digitalWrite(getPin(0), brightness);
}

void Led::setOn() { digitalWrite(getPin(0), LOW); }

void Led::setOff() { digitalWrite(getPin(0), HIGH); }
