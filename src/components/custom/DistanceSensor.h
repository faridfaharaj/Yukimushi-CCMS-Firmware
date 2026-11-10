#include "Component.h"
#include "pins/InputPinBase.h"
#include <VL53L0X.h>

class DistanceSensor : public Component<2> {
private:
  VL53L0X sensor;

public:
  explicit DistanceSensor(InputPinBase &scl, InputPinBase &sda)
      : Component<2>({&sda, &scl}) {}

  bool init() {
    Wire.begin(getPin(0)->getPin(), getPin(1)->getPin());

    if (!sensor.init()) {
      return false;
    }

    sensor.setTimeout(500);
    sensor.startContinuous();

    return true;
  }

  uint16_t readDistance() {
    uint16_t value = sensor.readRangeContinuousMillimeters();

    if (value == 64435) {
      value = -1;
    }

    return value;
  }

  float readDistanceCM() { return readDistance() / 10.0f; }

  bool timeoutOccurred() { return sensor.timeoutOccurred(); }
};
