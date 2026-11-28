#pragma once
#include <LittleFS.h>

inline bool due(unsigned long &last, unsigned long rate) {
  unsigned long now = millis();
  if (now - last < rate)
    return false;
  last = now;
  return true;
}

inline bool cooldownDue(unsigned long &last, unsigned long cooldown) {
  unsigned long now = millis();
  if (now - last < cooldown)
    return false;
  return true;
}

inline double sqrMagnitude(double x, double y, double z) {
  return x * x + y * y + z * z;
}

inline char *readFile(const char *path) {
  File f = LittleFS.open(path, "r");
  if (!f) {
    Serial.printf("Missing file: %s\n", path);
    return nullptr;
  }
  size_t len = f.size();
  char *buf = (char *)malloc(len + 1);
  if (!buf) {
    f.close();
    return nullptr;
  }
  size_t got = f.read((uint8_t *)buf, len);
  f.close();
  buf[got] = '\0';
  return buf;
}
