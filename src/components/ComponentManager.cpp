// NOTE: Pins were selected provisionally until the physical layout is done.
#include "ComponentManager.h"
#include "custom/pins/MultiplexerPins/PlexedInPin.h"
#include "custom/pins/regularPins/InPin.h"
#include "custom/pins/regularPins/OutPin.h"

DHT ComponentManager::tempHumSensor(D1, DHT11);

constexpr uint maxdistance = 400;
NewPing ComponentManager::distanceSensor(D2, D5, 400);

InPullupPin btnPin(D3);
Button ComponentManager::btn(btnPin, []() IRAM_ATTR {

});

OutPin ledPin(D4);
LED ComponentManager::led(ledPin);

InPin muxZ(A0);
OutPin muxS0(D6), muxS1(D7);
Multiplexer ComponentManager::multiplexer(muxZ, muxS0, muxS1);

PlexedInPin accX(ComponentManager::multiplexer, 0),
    accY(ComponentManager::multiplexer, 1),
    accZ(ComponentManager::multiplexer, 2);
Accelerometer ComponentManager::accelerometer(accX, accY, accZ);

PlexedInPin ldrPin(ComponentManager::multiplexer, 3);
LDR ComponentManager::ldr(ldrPin, 330, true);

void ComponentManager::initializeComponents() {
  tempHumSensor.begin();
  accelerometer.init();
  btn.init();
}
