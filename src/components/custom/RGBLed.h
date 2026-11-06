#pragma once
#include "Component.h"

class RGBLed : public Component<3> {
private:
  static short toRGBChannel(short brightness) {
    return brightness * 1023L / 255;
  }

public:
  explicit RGBLed(OutputPinBase &redPin, OutputPinBase &greenPin,
                  OutputPinBase &bluePin)
      : Component<3>({&redPin, &greenPin, &bluePin}) {}

  void setColor(uint32_t hexColor) {
    getPin(0)->writeAnalog(1023 - toRGBChannel((hexColor >> 16) & 0xFF));
    getPin(1)->writeAnalog(1023 - toRGBChannel((hexColor >> 8) & 0xFF));
    getPin(2)->writeAnalog(1023 - toRGBChannel(hexColor & 0xFF));
  }

  void setColor(uint16_t redVal, uint16_t greenVal, uint16_t blueVal) {
    getPin(0)->writeAnalog(redVal);
    getPin(1)->writeAnalog(greenVal);
    getPin(2)->writeAnalog(blueVal);
  }

  void setColor(uint8_t redVal, uint8_t greenVal, uint8_t blueVal) {
    getPin(0)->writeDigital(redVal);
    getPin(1)->writeDigital(greenVal);
    getPin(2)->writeDigital(blueVal);
  }

  void setRed() {
    getPin(0)->writeDigital(LOW);
    getPin(1)->writeDigital(HIGH);
    getPin(2)->writeDigital(HIGH);
  }

  void setGreen() {
    getPin(0)->writeDigital(HIGH);
    getPin(1)->writeDigital(LOW);
    getPin(2)->writeDigital(HIGH);
  }

  void setBlue() {
    getPin(0)->writeDigital(HIGH);
    getPin(1)->writeDigital(HIGH);
    getPin(2)->writeDigital(LOW);
  }

  void setOff() {
    getPin(0)->writeDigital(HIGH);
    getPin(1)->writeDigital(HIGH);
    getPin(2)->writeDigital(HIGH);
  }
};
