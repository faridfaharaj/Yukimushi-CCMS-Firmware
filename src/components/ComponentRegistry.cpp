// NOTE: Pins were selected provisionally until the physical layout is done.
#include "ComponentRegistry.h"
#include "custom/pins/MultiplexerPins/PlexedInPin.h"
#include "custom/pins/regularPins/InPin.h"
#include "custom/pins/regularPins/OutPin.h"

DHT ComponentRegistry::tempHumSensor(D1, DHT11);

constexpr uint maxdistance = 400;
NewPing ComponentRegistry::distanceSensor(D2, D5, 400);

InPullupPin btnPin(D3);
Button ComponentRegistry::btn(btnPin, []() IRAM_ATTR {

});

OutPin ledPin(D4);
LED ComponentRegistry::led(ledPin);

InPin muxZ(A0);
OutPin muxS0(D6), muxS1(D7);
Multiplexer ComponentRegistry::multiplexer(muxZ, muxS0, muxS1);

PlexedInPin accX(ComponentRegistry::multiplexer, 0),
    accY(ComponentRegistry::multiplexer, 1),
    accZ(ComponentRegistry::multiplexer, 2);
Accelerometer ComponentRegistry::accelerometer(accX, accY, accZ);

PlexedInPin ldrPin(ComponentRegistry::multiplexer, 3);
LDR ComponentRegistry::ldr(ldrPin, 330, true);

void ComponentRegistry::initializeComponents() {
  tempHumSensor.begin();
  accelerometer.init();
  btn.init();
}
