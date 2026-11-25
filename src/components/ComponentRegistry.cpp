// NOTE: Pins were selected provisionally until the physical layout is done.
#include "ComponentRegistry.h"
#include "custom/pins/MultiplexerPins/PlexedInPin.h"
#include "custom/pins/regularPins/InPin.h"
#include "custom/pins/regularPins/OutPin.h"

namespace ComponentRegistry {

DHT tempHumSensor(D1, DHT11);

InPin sclPin(D2), sdaPin(D5);
DistanceSensor distanceSensor(sclPin, sdaPin);

InPullupPin btnPin(D3);
Button btn(btnPin);

InPin muxZ(A0);
OutPin muxS0(D2), muxS1(D3);
Multiplexer multiplexer(muxZ, muxS0, muxS1);

PlexedInPin accX(multiplexer, 0), accY(multiplexer, 1), accZ(multiplexer, 2);
Accelerometer accelerometer(accX, accY, accZ);

PlexedInPin ldrPin(multiplexer, 3);
LDR ldr(ldrPin, 330, true);

SoftwareSerial bluetooth(4, 5);

void initializeComponents() {
  bluetooth.begin(9600);
  tempHumSensor.begin();
  accelerometer.init();
  btn.init();
  distanceSensor.init();
}

} // namespace ComponentRegistry
