#pragma once
#include "Reading.h"

class CoverReading : public Reading {
private:
  bool open;

public:
  explicit CoverReading(bool open = false) : Reading(), open(open) {}

  bool getOpen() const { return open; }

  size_t serialize(uint8_t *buffer) const override {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &open, sizeof(open));

    return offset + sizeof(open);
  }

  size_t size() const override { return Reading::size() + sizeof(open); }
};
