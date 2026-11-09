#pragma once

inline bool due(unsigned long &last, unsigned long rate) {
  unsigned long now = millis();
  if (now - last < rate)
    return false;
  last = now;
  return true;
}

inline bool cooldownDue(unsigned long &last, unsigned long cooldown) {
  unsigned long now = millis();
  if (now - last < cooldown) return false;
  return true;
}

inline double sqrMagnitude(double x, double y, double z) {
  return x * x + y * y + z * z;
}
