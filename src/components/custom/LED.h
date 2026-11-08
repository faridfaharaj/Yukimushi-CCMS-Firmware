#pragma once
#include "Component.h"
#include "pins/OutputPinBase.h"

class LED : public Component<1> {
public:
  explicit LED(OutputPinBase &pin) : Component<1>({&pin}) {}

  void setBrightness(uint16_t brightness) {
    getPin(0)->writeAnalog(brightness);
  }
  void setState(uint8_t state) { getPin(0)->writeDigital(state); }

  void setOn() { getPin(0)->writeDigital(LOW); }
  void setOff() { getPin(0)->writeDigital(HIGH); }
};
