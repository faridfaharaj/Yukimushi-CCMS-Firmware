#pragma once
#include "pin.h"
#include <cstddef>

template <std::size_t N> class Component {
public:
  explicit Component(Pin *const (&p)[N]) {
    for (std::size_t i = 0; i < N; ++i) {
      pins[i] = p[i];
      pins[i]->begin();
    }
  }

protected:
  Pin *getPin(int idx) { return pins[idx]; }

private:
  Pin *pins[N];
};
