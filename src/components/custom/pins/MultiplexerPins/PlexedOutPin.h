#pragma once
#include "../OutputPinBase.h"

class PlexedOutPin : public OutputPinBase {
  ChannelSource &src;
  uint8_t ch;

public:
  PlexedOutPin(ChannelSource &src, uint8_t ch) : src(src), ch(ch) {}
  void writeAnalog(int val) override { src.writeAnalog(ch, val); }
  void writeDigital(int val) override { src.writeDigital(ch, val); }
  uint8_t getPin() override { return src.getZPin(ch); }
};
