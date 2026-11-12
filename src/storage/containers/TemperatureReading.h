#pragma once
#include "Reading.h"

class TemperatureReading : public Reading {
private:
  float celsius;

public:
  explicit TemperatureReading(unsigned long stamp, float degrees)
      : Reading(stamp), celsius(degrees) {}

  explicit TemperatureReading() : Reading(0), celsius(0.0f) {}

  float getDegrees() const { return celsius; }

  float getFahrenheit() const { return celsius * 1.8f + 32.0f; }

  size_t serialize(uint8_t *buffer) const {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &celsius, sizeof(celsius));

    return offset + sizeof(celsius);
  }

  static constexpr size_t size() { return Reading::size() + sizeof(float); }

  static constexpr const char *getName() { return "TemperatureReading"; }
};
