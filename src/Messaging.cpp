#include "Messaging.h"
#include "CommsManager.h"
#include "SensorProcesses.h"
#include "storage/InMemoryStorage.h"
#include <ESP8266WiFi.h>

namespace Messaging {

WiFiEventHandler onConnected;

template <typename T> bool publishReading(T &reading) {
  uint8_t buffer[reading.size()];
  size_t n = reading.serialize(buffer);

  return CommsManager::mqttConnected() && CommsManager::mqttPublish(buffer, n);
}

template <typename T> void processReading(T &reading) {
  if (!publishReading(reading)) {
    InMemoryStorage::store(reading);
  }
}

template <typename T> void publishStored() {
  T reading;
  do {
    while (InMemoryStorage::load(reading)) {
      if (!publishReading(reading)) {
        InMemoryStorage::store(reading);
        return;
      }
      yield();
    }
  } while (InMemoryStorage::fill(reading));
}

void publishStorage() {
  publishStored<HumidityReading>();
  publishStored<TemperatureReading>();
  publishStored<LightReading>();
  publishStored<CoverReading>();
  publishStored<ShockReading>();
  publishStored<TippedReading>();
}

} // namespace Messaging

void Messaging::init() {
  CommsManager::onMQTTconnected.subscribe(publishStorage);
  SensorProcesses::onReadHumidity.subscribe(processReading<HumidityReading>);
  SensorProcesses::onReadLight.subscribe(processReading<LightReading>);
  SensorProcesses::onReadTemperature.subscribe(
      processReading<TemperatureReading>);

  SensorProcesses::onCoverChanged.subscribe(processReading<CoverReading>);
  SensorProcesses::onShock.subscribe(processReading<ShockReading>);
  SensorProcesses::onTipped.subscribe(processReading<TippedReading>);
}
