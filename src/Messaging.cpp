#include "Messaging.h"
#include "CommsManager.h"
#include "SensorProcesses.h"

namespace Messaging {

void publishCover(CoverReading coverR) {
  if (!CommsManager::mqttConnected())
    return;

  uint8_t buffer[coverR.size()];
  size_t n = coverR.serialize(buffer);
  CommsManager::mqttPublish(buffer, n);
}

void publishHumidity(HumidityReading humidityR) {
  if (!CommsManager::mqttConnected())
    return;

  uint8_t buffer[humidityR.size()];
  size_t n = humidityR.serialize(buffer);

  if (!CommsManager::mqttPublish(buffer, n)) {
    return;
  }
}

void publishLight(LightReading lightR) {
  if (!CommsManager::mqttConnected())
    return;

  uint8_t buffer[lightR.size()];
  size_t n = lightR.serialize(buffer);
  if (!CommsManager::mqttPublish(buffer, n)) {
    return;
  }
}

void publishShock(ShockReading shockR) {
  if (!CommsManager::mqttConnected())
    return;

  uint8_t buffer[shockR.size()];
  size_t n = shockR.serialize(buffer);
  if (!CommsManager::mqttPublish(buffer, n)) {
    return;
  }
}

void publishTemperature(TemperatureReading temperatureR) {
  if (!CommsManager::mqttConnected())
    return;

  uint8_t buffer[temperatureR.size()];
  size_t n = temperatureR.serialize(buffer);
  if (!CommsManager::mqttPublish(buffer, n)) {
    return;
  }
}

void publishTipped(TippedReading tippedR) {
  if (!CommsManager::mqttConnected())
    return;

  uint8_t buffer[tippedR.size()];
  size_t n = tippedR.serialize(buffer);
  if (!CommsManager::mqttPublish(buffer, n)) {
    return;
  }
}

} // namespace Messaging

void Messaging::init() {
  SensorProcesses::onReadHumidity.subscribe(publishHumidity);
  SensorProcesses::onReadLight.subscribe(publishLight);
  SensorProcesses::onReadTemperature.subscribe(publishTemperature);

  SensorProcesses::onCoverChanged.subscribe(publishCover);
  SensorProcesses::onShock.subscribe(publishShock);
  SensorProcesses::onTipped.subscribe(publishTipped);
}
