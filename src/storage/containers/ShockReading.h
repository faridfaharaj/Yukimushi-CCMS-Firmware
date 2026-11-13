#pragma once
#include "Reading.h"

class ShockReading : public Reading {
public:
  explicit ShockReading(unsigned long stamp = 0) : Reading(stamp) {};
};
