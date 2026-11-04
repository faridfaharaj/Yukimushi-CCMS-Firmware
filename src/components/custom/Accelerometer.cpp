#include "Accelerometer.h"
#include <cstdint>

Accelerometer::Accelerometer(uint8_t analog_pin, uint8_t multiplexA,
                             uint8_t multiplexB)
    : Component<3>(
          {{analog_pin, INPUT}, {multiplexA, OUTPUT}, {multiplexB, OUTPUT}}) {};

int Accelerometer::readChannel(bool s1, bool s0) {
  digitalWrite(getPin(1), s1);
  digitalWrite(getPin(2), s0);
  delay(10);
  return analogRead(getPin(0));
}

float Accelerometer::calcAcceleration(int raw, float zero) {
  float voltage = raw * ADC_REF / ADC_AMPLITUDE;
  return (voltage - zero) / SENSITIVITY;
}

float Accelerometer::getAccelerationX() {
  return calcAcceleration(readChannel(LOW, LOW), ZERO_X);
}

float Accelerometer::getAccelerationY() {
  return calcAcceleration(readChannel(LOW, HIGH), ZERO_Y);
}

float Accelerometer::getAccelerationZ() {
  return calcAcceleration(readChannel(HIGH, LOW), ZERO_Z);
}

int Accelerometer::getPotentiometer() { return readChannel(HIGH, HIGH); }
