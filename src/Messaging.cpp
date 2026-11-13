#include "Messaging.h"
#include "CommsManager.h"
#include "SensorProcesses.h"
#include "storage/InMemoryStorage.h"

namespace Messaging {

template <typename T> void publish(T &reading) {
  if (CommsManager::mqttConnected()) {
    uint8_t buffer[reading.size()];
    size_t n = reading.serialize(buffer);
    if (CommsManager::mqttPublish(buffer, n))
      return;
  }

  InMemoryStorage::store(reading);
}

} // namespace Messaging

void Messaging::init() {
  SensorProcesses::onReadHumidity.subscribe(publish<HumidityReading>);
  SensorProcesses::onReadLight.subscribe(publish<LightReading>);
  SensorProcesses::onReadTemperature.subscribe(publish<TemperatureReading>);

  SensorProcesses::onCoverChanged.subscribe(publish<CoverReading>);
  SensorProcesses::onShock.subscribe(publish<ShockReading>);
  SensorProcesses::onTipped.subscribe(publish<TippedReading>);
}
