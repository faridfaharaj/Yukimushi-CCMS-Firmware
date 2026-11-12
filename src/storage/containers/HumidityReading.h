#pragma once
#include "Reading.h"

class HumidityReading : public Reading {
private:
  float humidityPercentage;

public:
  explicit HumidityReading(unsigned long stamp, float humidityPercentage)
      : Reading(stamp), humidityPercentage(humidityPercentage) {}

  explicit HumidityReading() : Reading(0), humidityPercentage(0.0f) {}

  float getHumidityPercentage() const { return humidityPercentage; }

  size_t serialize(uint8_t *buffer) const {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &humidityPercentage, sizeof(humidityPercentage));

    return offset + sizeof(humidityPercentage);
  }

  static constexpr size_t size() { return Reading::size() + sizeof(float); }

  static constexpr const char *getName() { return "Humidity"; }
};
