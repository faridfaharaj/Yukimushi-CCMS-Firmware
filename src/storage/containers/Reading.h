#pragma once
#include <Arduino.h>

class Reading {
private:
  unsigned long timeStamp;

public:
  Reading() : timeStamp(millis()) {}

  virtual ~Reading() = default;

  unsigned long getTimeStamp() const { return timeStamp; }

  virtual size_t serialize(uint8_t *buffer) const {
    memcpy(buffer, &timeStamp, sizeof(timeStamp));
    return sizeof(timeStamp);
  }

  virtual size_t size() const { return sizeof(timeStamp); }
};
