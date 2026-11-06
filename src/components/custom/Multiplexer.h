#pragma once
#include "Component.h"

class Multiplexer : Component<3>, public ChannelSource {
public:
  Multiplexer(Pin &z, OutputPinBase &s0, OutputPinBase &s1)
      : Component<3>({&z, &s0, &s1}) {}

  int readAnalog(uint8_t ch) override {
    select(ch);
    return getPin(0)->readAnalog();
  }
  int readDigital(uint8_t ch) override {
    select(ch);
    return getPin(0)->readDigital();
  }
  void writeAnalog(uint8_t ch, int v) override {
    select(ch);
    getPin(0)->writeAnalog(v);
  }
  void writeDigital(uint8_t ch, int v) override {
    select(ch);
    getPin(0)->writeDigital(v);
  }

private:
  uint8_t current = 255;
  void select(uint8_t ch) {
    if (ch == current)
      return;
    getPin(1)->writeDigital(ch & 1);
    getPin(2)->writeDigital((ch >> 1) & 1);
    current = ch;
    delayMicroseconds(200);
  }
};
