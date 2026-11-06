#include "ComponentManager.h"


outPin ledR(D2), ledG(D3), ledB(D4);
RGBLed ComponentManager::led(ledR, ledG, ledB);

inPin ldrPin(A0);
LDR ComponentManager::ldr(ldrPin, 330, true);

/*
inPin muxZ(A0);
outPin muxS0(D3), muxS1(D4);
Multiplexer ComponentManager::multiplexer(muxZ, muxS0, muxS1);
PlexedInPin accX(ComponentManager::multiplexer, 0),
    accY(ComponentManager::multiplexer, 1),
    accZ(ComponentManager::multiplexer, 2);

Accelerometer ComponentManager::accelerometer(accX, accY, accZ);
*/

void ComponentManager::initializeComponents() {
  /*
  tempHumSensor.begin();
  accelerometer.init();
  */
}
