#pragma once
#include "Reading.h"

class __attribute__((packed)) CoverReading : public Reading {
private:
  bool open;

public:
  explicit CoverReading(unsigned long stamp, bool open)
      : Reading(stamp), open(open) {}

  explicit CoverReading() : Reading(0), open(false) {}

  bool getOpen() const { return open; }

  size_t serialize(uint8_t *buffer) const {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &open, sizeof(open));

    return offset + sizeof(open);
  }

  static constexpr size_t size() { return Reading::size() + sizeof(bool); }

  static constexpr const char *getName() { return "Cover"; }
};
