#pragma once
#include "Component.h"
#include "pin.h"
#include <Schedule.h>

class Button : public Component<1> {
private:
  void (*callback)();

  volatile unsigned long last = 0;

  static void IRAM_ATTR onInputChanged(void *arg) {
    Button *b = static_cast<Button *>(arg);
    unsigned long now = millis();
    if (now - b->last > 50) {
      b->last = now;
      schedule_function(b->callback);
    }
  }

public:
  explicit Button(inPullupPin &pin, void (*callback)())
      : Component<1>({&pin}), callback(callback) {}

  void init() {
    attachInterruptArg(getPin(0)->getPin(), onInputChanged, this, FALLING);
  }
};
