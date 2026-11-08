// INFO: only works on (D1, D2, D5, D6, D7) pins
#pragma once
#include "../../Event.h"
#include "Component.h"
#include "pins/regularPins/InPullupPin.h"
#include <Schedule.h>

class Button : public Component<1> {
private:
  volatile unsigned long last = 0;
  volatile unsigned long pressStart = 0;
  unsigned long duration = 0;

  static void IRAM_ATTR onInputChanged(void *arg) {
    Button *b = static_cast<Button *>(arg);
    unsigned long now = millis();
    if (now - b->last < 50)
      return;
    b->last = now;

    if (digitalRead(b->getPin(0)->getPin()) == LOW) {
      b->pressStart = now;
    } else {
      b->duration = now - b->pressStart;
      schedule_function(std::bind(&Button::fire, b));
    }
  }

  void fire() { onRelease.emit(duration); }

public:
  Event<4, unsigned long> onRelease;

  explicit Button(InPullupPin &pin) : Component<1>({&pin}) {}

  void init() {
    attachInterruptArg(getPin(0)->getPin(), onInputChanged, this, CHANGE);
  }
};
