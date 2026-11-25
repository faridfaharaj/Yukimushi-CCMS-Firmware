#pragma once
#include "Reading.h"

class TippedReading : public Reading {
public:
  explicit TippedReading(unsigned long stamp = 0) : Reading(stamp) {};

  static TippedReading deserialize(const uint8_t *buffer) {
    unsigned long stamp;
    memcpy(&stamp, buffer, sizeof(stamp));

    return TippedReading(stamp);
  }

  static constexpr const char *getName() { return "Tipped"; }
};
