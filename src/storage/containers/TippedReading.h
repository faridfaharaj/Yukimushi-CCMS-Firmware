#pragma once
#include "Reading.h"

class TippedReading : public Reading {
public:
  explicit TippedReading(unsigned long stamp = 0) : Reading(stamp) {};

  static constexpr const char *getName() { return "TippedReading"; }
};
