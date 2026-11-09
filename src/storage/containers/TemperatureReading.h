#pragma once
#include "Reading.h"

class TemperatureReading : public Reading {
private:
  float celsius;

public:
  explicit TemperatureReading(float degrees = 0.0f)
      : Reading(), celsius(degrees) {}

  float getDegrees() const { return celsius; }

  float getFahrenheit() const { return celsius * 1.8f + 32.0f; }

  size_t serialize(uint8_t *buffer) const override {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &celsius, sizeof(celsius));

    return offset + sizeof(celsius);
  }

  size_t size() const override { return Reading::size() + sizeof(celsius); }
};
