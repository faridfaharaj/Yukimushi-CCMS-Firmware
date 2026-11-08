// NOTE: Pins were selected provisionally until the physical layout is done.
#include "ComponentRegistry.h"
#include "custom/pins/MultiplexerPins/PlexedInPin.h"
#include "custom/pins/regularPins/InPin.h"
#include "custom/pins/regularPins/OutPin.h"

namespace ComponentRegistry {

constexpr unsigned int MAX_DISTANCE = 400;
DHT tempHumSensor(D1, DHT11);
NewPing distanceSensor(D2, D5, MAX_DISTANCE);

InPullupPin btnPin(D3);
Button btn(btnPin);

OutPin ledPin(D4);
LED led(ledPin);

InPin muxZ(A0);
OutPin muxS0(D6), muxS1(D7);
Multiplexer multiplexer(muxZ, muxS0, muxS1);

PlexedInPin accX(multiplexer, 0), accY(multiplexer, 1), accZ(multiplexer, 2);
Accelerometer accelerometer(accX, accY, accZ);

PlexedInPin ldrPin(multiplexer, 3);
LDR ldr(ldrPin, 330, true);

void initializeComponents() {
  tempHumSensor.begin();
  accelerometer.init();
  btn.init();
}

} // namespace ComponentRegistry
