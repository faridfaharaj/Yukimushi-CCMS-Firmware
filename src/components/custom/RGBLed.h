#pragma once
#include "Component.h"
#include <Arduino.h>

class RGBLed : public Component<3> {
public:
  RGBLed(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);
  void setColor(uint32_t hex_color);
  void setColor(uint16_t red_val, uint16_t green_val, uint16_t blue_val);
  void setColor(uint8_t red_val, uint8_t green_val, uint8_t blue_val);
  void setRed();
  void setGreen();
  void setBlue();
  void setOff();

};
