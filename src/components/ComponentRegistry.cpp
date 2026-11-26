// NOTE: Pins were selected provisionally until the physical layout is done.
#include "ComponentRegistry.h"
#include "custom/pins/MultiplexerPins/PlexedInPin.h"
#include "custom/pins/regularPins/InPin.h"
#include "custom/pins/regularPins/OutPin.h"

namespace ComponentRegistry {

DHT tempHumSensor(D1, DHT11);

NewPing distanceSensor(D4, D7);

InPin muxZ(A0);
OutPin muxS0(D2), muxS1(D3);
Multiplexer multiplexer(muxZ, muxS0, muxS1);

PlexedInPin accX(multiplexer, 0), accY(multiplexer, 1), accZ(multiplexer, 2);
Accelerometer accelerometer(accX, accY, accZ);

PlexedInPin ldrPin(multiplexer, 3);
LDR ldr(ldrPin, 330, true);

SoftwareSerial bluetooth(D0, D8);

UninitializedPin geoTX(D5);
UninitializedPin geoRX(D6);
GPS gps(geoRX, geoTX);

void initializeComponents() {
  gps.init(9600);
  tempHumSensor.begin();
  accelerometer.init();
}

} // namespace ComponentRegistry
