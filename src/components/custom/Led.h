#pragma once
#include "Component.h"

class Led : public Component<1> {
public:
  Led(uint8_t pin);
  void setBrightness(uint16_t brightness);
  void setBrightness(uint8_t brightness);
  void setOn();
  void setOff();

};
