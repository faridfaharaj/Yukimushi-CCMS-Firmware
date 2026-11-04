#pragma once
#include <Arduino.h>
#include <cstddef>
#include <cstdint>

struct Pin {
  uint8_t pin;
  uint8_t mode;
};

template <std::size_t N> class Component{
public:
  explicit Component(const Pin (&pins)[N]);

protected:
  uint8_t getPin(int idx) const;

private:
  uint8_t pins[N];
};
