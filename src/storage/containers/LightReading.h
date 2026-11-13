#pragma once
#include "Reading.h"

class LightReading : public Reading {
private:
  float lux;

public:
  explicit LightReading(unsigned long stamp, float lux)
      : Reading(stamp), lux(lux) {}

  explicit LightReading() : Reading(0), lux(0.0f) {}

  float getLux() const { return lux; }

  size_t serialize(uint8_t *buffer) const {
    size_t offset = Reading::serialize(buffer);
    memcpy(buffer + offset, &lux, sizeof(lux));
    return offset + sizeof(lux);
  }

  static constexpr size_t size() { return Reading::size() + sizeof(float); }
};
