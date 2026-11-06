#pragma once
#include "../InputPinBase.h"

class PlexedInPin : public InputPinBase {
  ChannelSource &src;
  uint8_t ch;

public:
  PlexedInPin(ChannelSource &src, uint8_t ch) : src(src), ch(ch) {}
  int readAnalog() override { return src.readAnalog(ch); }
  int readDigital() override { return src.readDigital(ch); }
};
