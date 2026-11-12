#pragma once
#include "Reading.h"

class TippedReading : public Reading {
public:
<<<<<<< HEAD
  explicit TippedReading(unsigned long stamp = 0) : Reading(stamp) {};

  static constexpr const char *getName() { return "TippedReading"; }
=======
  static constexpr const char *getName() { return "Tipped"; }
>>>>>>> 69ff013 (removed string concatenation to DiskStorage)
};
