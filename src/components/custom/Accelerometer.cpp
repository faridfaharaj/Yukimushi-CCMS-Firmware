#include "Accelerometer.h"
#include <cstdint>

Accelerometer::Accelerometer(uint8_t analog_pin, uint8_t multiplexA,
                             uint8_t multiplexB)
    : Component<3>(
          {{analog_pin, INPUT}, {multiplexA, OUTPUT}, {multiplexB, OUTPUT}}) {};

int Accelerometer::getXAxis() { return analogRead(getPin(0)); };
int Accelerometer::getYAxis() { return analogRead(getPin(1)); };
int Accelerometer::getZAxis() { return analogRead(getPin(2)); };

float Accelerometer::getAccelerationX() {
  digitalWrite(getPin(1), LOW);
  digitalWrite(getPin(2), LOW);
  delay(10);
  return calcAcceleration((float)getXAxis());
}
float Accelerometer::getAccelerationY() {
  digitalWrite(getPin(1), LOW);
  digitalWrite(getPin(2), HIGH);
  delay(10);
  return calcAcceleration((float)getYAxis());
}
float Accelerometer::getAccelerationZ() {
  digitalWrite(getPin(1), HIGH);
  digitalWrite(getPin(2), HIGH);
  delay(10);
  return calcAcceleration((float)getZAxis());
}

float calcAcceleration(float signal) {
  float voltage = signal * ADC_REF / ADC_AMPLITUDE;
  return (voltage - ZERO_X) / SENSITIVITY;
}
