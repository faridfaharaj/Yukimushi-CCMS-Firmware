#include "Component.h"
#include <cstdint>

template <std::size_t N> Component<N>::Component(const Pin (&pins)[N]) {
  // Initialize pins
  for (std::size_t i = 0; i < N; ++i) {
    pinMode(pins[i].pin, pins[i].mode);
    this->pins[i] = pins[i].pin;
  }
}

template <std::size_t N> uint8_t Component<N>::getPin(int idx) const {
  return this->pins[idx];
}

template class Component<1>;
template class Component<2>;
template class Component<3>;
template class Component<8>;
