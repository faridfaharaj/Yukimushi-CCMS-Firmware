#pragma once
#include "Reading.h"

class __attribute__((packed)) CoverReading : public Reading {
private:
  bool open;

public:
  explicit CoverReading(bool open = false) : Reading(), open(open) {}

  bool getOpen() const { return open; }

  size_t serialize(uint8_t *buffer) const {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &open, sizeof(open));

    return offset + sizeof(open);
  }

  static constexpr size_t size() { return Reading::size() + sizeof(bool); }
};
