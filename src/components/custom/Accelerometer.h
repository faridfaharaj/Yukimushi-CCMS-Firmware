// INFO: only works on analog (A0) pin, use a multiplexer
#pragma once
#include "Component.h"
#include "pins/InputPinBase.h"

class Accelerometer : Component<3> {
  static inline float ZERO_X = 0, ZERO_Y = 0, ZERO_Z = 0;
  static constexpr float SENSITIVITY = 0.33f;
  static constexpr float ADC_REF = 3.3f;
  static constexpr float ADC_MAX = 1023.0f;

public:
  explicit Accelerometer(InputPinBase &outX, InputPinBase &outY,
                         InputPinBase &outZ)
      : Component<3>({&outX, &outY, &outZ}) {};

  float calcAcceleration(int raw, float zero) {
    return (calcVolts(raw) - zero) / SENSITIVITY;
  }

  float calcVolts(int raw) { return raw * ADC_REF / ADC_MAX; }

  void init() {
    long x = 0, y = 0, z = 0;
    for (int i = 0; i < 32; i++) {
      x += getPin(0)->readAnalog();
      y += getPin(1)->readAnalog();
      z += getPin(2)->readAnalog();
      delay(5);
    }
    ZERO_X = (x / 32.0f) * ADC_REF / ADC_MAX;
    ZERO_Y = (y / 32.0f) * ADC_REF / ADC_MAX;
    ZERO_Z = (z / 32.0f) * ADC_REF / ADC_MAX;
  }

  float getAccelerationX() {
    return calcAcceleration(getPin(0)->readAnalog(), ZERO_X);
  }
  float getAccelerationY() {
    return calcAcceleration(getPin(1)->readAnalog(), ZERO_Y);
  }
  float getAccelerationZ() {
    return calcAcceleration(getPin(2)->readAnalog(), ZERO_Z);
  }

  void readAcceleration(float &outX, float &outY, float &outZ) {
    outX = getAccelerationX();
    outY = getAccelerationY();
    outZ = getAccelerationZ();
  }
};
