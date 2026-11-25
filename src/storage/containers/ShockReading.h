#pragma once
#include "Reading.h"

class ShockReading : public Reading {
public:
  explicit ShockReading(unsigned long stamp = 0) : Reading(stamp) {};

  static ShockReading deserialize(const uint8_t *buffer) {
    unsigned long stamp;
    memcpy(&stamp, buffer, sizeof(stamp));

    return ShockReading(stamp);
  }

  static constexpr const char *getName() { return "Shock"; }
};
