#pragma once
#include <Arduino.h>

template <size_t N, typename... Args> class Event {
public:
  using Handler = void (*)(Args...);

  bool subscribe(Handler f) {
    if (count_ >= N)
      return false;
    handlers_[count_++] = f;
    return true;
  }

  void emit(Args... args) {
    for (size_t i = 0; i < count_; i++)
      handlers_[i](args...);
  }

private:
  Handler handlers_[N];
  size_t count_ = 0;
};
