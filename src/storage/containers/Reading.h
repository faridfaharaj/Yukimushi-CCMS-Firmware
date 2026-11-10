#pragma once
#include <Arduino.h>

class Reading {
private:
  unsigned long timeStamp;

public:
  Reading() : timeStamp(millis()) {}

  unsigned long getTimeStamp() const { return timeStamp; }

  size_t serialize(uint8_t *buffer) const {
    memcpy(buffer, &timeStamp, sizeof(timeStamp));
    return sizeof(timeStamp);
  }

  static constexpr size_t size() { return sizeof(unsigned long); }
};
