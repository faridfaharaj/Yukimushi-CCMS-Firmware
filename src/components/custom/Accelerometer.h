#include "Component.h"
#include <cstdint>

#define ZERO_X 1.22
#define ZERO_Y 1.22
#define ZERO_Z 1.25

#define SENSITIVITY 0.25

#define ADC_AMPLITUDE 1024

#define ADC_REF 5

#define VREF 3300.0
#define ADC_MAX 1024

class Accelerometer : Component<3> {

public:
  Accelerometer(uint8_t analg_pin, uint8_t pin_2, uint8_t pin_3);

  float getAccelerationX();
  float getAccelerationY();
  float getAccelerationZ();

  float calcVolt(float);
  float calcAcceleration(float);

private:
  int getXAxis();
  int getYAxis();
  int getZAxis();
};
