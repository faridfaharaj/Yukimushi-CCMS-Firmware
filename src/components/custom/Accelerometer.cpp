#include "Accelerometer.h"
#include <cstdint>

Accelerometer::Accelerometer(uint8_t analg_pin, uint8_t pin_2, uint8_t pin_3)
    : Component<3>({{analg_pin, INPUT}, {pin_2, OUTPUT}, {pin_3, OUTPUT}}) {};

int Accelerometer::getXAxis() { return analogRead(getPin(0)); };
int Accelerometer::getYAxis() { return analogRead(getPin(1)); };
int Accelerometer::getZAxis() { return analogRead(getPin(2)); };

float Accelerometer::getAccelerationX() {
  digitalWrite(getPin(1) ,LOW);
  digitalWrite(getPin(2),LOW);
  return calcAcceleration((float)getXAxis());
}
float Accelerometer::getAccelerationY() {
  digitalWrite(getPin(1) ,LOW);
  digitalWrite(getPin(2),LOW);
  return calcAcceleration((float)getYAxis());
}
float Accelerometer::getAccelerationZ() {
  digitalWrite(getPin(1) ,LOW);
  digitalWrite(getPin(2),LOW);
  return calcAcceleration((float)getZAxis());
}

float calcAcceleration(float signal) {
  float voltage = signal * ADC_REF / ADC_AMPLITUDE;
  return (voltage - ZERO_X) / SENSITIVITY;
}
