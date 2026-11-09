#pragma once
#include "Reading.h"

class HumidityReading : public Reading {
private:
  float humidityPercentage;

public:
  explicit HumidityReading(float humidityPercentage = 0.0f)
      : Reading(), humidityPercentage(humidityPercentage) {}

  float getHumidityPercentage() const { return humidityPercentage; }

  size_t serialize(uint8_t *buffer) const override {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &humidityPercentage, sizeof(humidityPercentage));

    return offset + sizeof(humidityPercentage);
  }

  size_t size() const override {
    return Reading::size() + sizeof(humidityPercentage);
  }
};
