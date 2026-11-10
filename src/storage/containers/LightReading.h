#pragma once
#include "Reading.h"

class LightReading : public Reading {
private:
  float lux;

public:
  explicit LightReading(float lux = 0.0f) : Reading(), lux(lux) {}

  float getLux() const { return lux; }

  size_t serialize(uint8_t *buffer) const {
    size_t offset = Reading::serialize(buffer);
    memcpy(buffer + offset, &lux, sizeof(lux));
    return offset + sizeof(lux);
  }

  static constexpr size_t size() { return Reading::size() + sizeof(float); }
};
