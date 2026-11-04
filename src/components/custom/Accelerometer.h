#include "Component.h"
#include <cstdint>

#define ZERO_X 1.65f // volts, adjust to your hardware
#define ZERO_Y 1.65f
#define ZERO_Z 1.65f

#define SENSITIVITY 0.300f // V/g
//
#define ADC_AMPLITUDE 1024.0f
#define ADC_REF 3.3f

class Accelerometer : Component<3> {

public:
  Accelerometer(uint8_t analg_pin, uint8_t pin_2, uint8_t pin_3);

  float getAccelerationX();
  float getAccelerationY();
  float getAccelerationZ();
  int getPotentiometer();

private:
  int readChannel(bool s1, bool s0);
  float calcAcceleration(int raw, float zero);
};
