#include "RGBLed.h"

RGBLed::RGBLed(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : Component<3>(
          {{red_pin, OUTPUT}, {green_pin, OUTPUT}, {blue_pin, OUTPUT}}) {}

short toRGBChannel(short brightness) { return brightness * 1023L / 255; }

void RGBLed::setColor(uint32_t hex_color) {
  analogWrite(getPin(0), 1023 - toRGBChannel((hex_color >> 16) & 0xFF));
  analogWrite(getPin(1), 1023 - toRGBChannel((hex_color >> 8) & 0xFF));
  analogWrite(getPin(2), 1023 - toRGBChannel(hex_color & 0xFF));
}

void RGBLed::setColor(uint16_t red_val, uint16_t green_val, uint16_t blue_val) {
  analogWrite(getPin(0), red_val);
  analogWrite(getPin(1), green_val);
  analogWrite(getPin(2), blue_val);
}

void RGBLed::setColor(uint8_t red_val, uint8_t green_val, uint8_t blue_val) {
  digitalWrite(getPin(0), red_val);
  digitalWrite(getPin(1), green_val);
  digitalWrite(getPin(2), blue_val);
}

void RGBLed::setRed() {
  digitalWrite(getPin(0), LOW);
  digitalWrite(getPin(1), HIGH);
  digitalWrite(getPin(2), HIGH);
}

void RGBLed::setGreen() {
  digitalWrite(getPin(0), HIGH);
  digitalWrite(getPin(1), LOW);
  digitalWrite(getPin(2), HIGH);
}

void RGBLed::setBlue() {
  digitalWrite(getPin(0), HIGH);
  digitalWrite(getPin(1), HIGH);
  digitalWrite(getPin(2), LOW);
}

void RGBLed::setOff() {
  digitalWrite(getPin(0), HIGH);
  digitalWrite(getPin(1), HIGH);
  digitalWrite(getPin(2), HIGH);
}
