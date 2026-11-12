#pragma once
#include "Reading.h"

class ShockReading : public Reading {
public:
<<<<<<< HEAD
  explicit ShockReading(unsigned long stamp = 0) : Reading(stamp) {};

  static constexpr const char *getName() { return "ShockReading"; }
=======
  static constexpr const char *getName() { return "Shock"; }
>>>>>>> 69ff013 (removed string concatenation to DiskStorage)
};
