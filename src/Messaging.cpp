#include "Messaging.h"
#include "CommsManager.h"
#include "SensorProcesses.h"
#include "storage/InMemoryStorage.h"
#include <ESP8266WiFi.h>

namespace Messaging {

const char *channelTopicCover = nullptr;
const char *channelTopicHumidity = nullptr;
const char *channelTopicLight = nullptr;
const char *channelTopicShock = nullptr;
const char *channelTopicTemperature = nullptr;
const char *channelTopicTipped = nullptr;

WiFiEventHandler onConnected;

template <typename T> bool publishReadingBase(T &reading, const char *topic) {
  uint8_t buffer[reading.size()];
  size_t n = reading.serialize(buffer);

  return CommsManager::wifiConnected() && CommsManager::mqttConnected() &&
         CommsManager::mqttPublish(topic, buffer, n);
}

bool publishReading(HumidityReading &reading) {
  return publishReadingBase(reading, channelTopicCover);
}
bool publishReading(TemperatureReading &reading) {
  return publishReadingBase(reading, channelTopicTemperature);
}
bool publishReading(LightReading &reading) {
  return publishReadingBase(reading, channelTopicLight);
}
bool publishReading(CoverReading &reading) {
  return publishReadingBase(reading, channelTopicCover);
}
bool publishReading(ShockReading &reading) {
  return publishReadingBase(reading, channelTopicShock);
}
bool publishReading(TippedReading &reading) {
  return publishReadingBase(reading, channelTopicTipped);
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

} // namespace Messaging

void Messaging::publishStorage() {
  publishStored<HumidityReading>();
  publishStored<TemperatureReading>();
  publishStored<LightReading>();
  publishStored<CoverReading>();
  publishStored<ShockReading>();
  publishStored<TippedReading>();
}

void Messaging::init(const char *topicCover, const char *topicHumidity,
                     const char *topicLight, const char *topicShock,
                     const char *topicTemperature, const char *topicTipped) {

  channelTopicCover = strdup(topicCover);
  channelTopicHumidity = strdup(topicHumidity);
  channelTopicLight = strdup(topicLight);
  channelTopicShock = strdup(topicShock);
  channelTopicTemperature = strdup(topicTemperature);
  channelTopicTipped = strdup(topicTipped);

  CommsManager::onMQTTconnected.subscribe(publishStorage);
  SensorProcesses::onReadHumidity.subscribe(processReading<HumidityReading>);
  SensorProcesses::onReadLight.subscribe(processReading<LightReading>);
  SensorProcesses::onReadTemperature.subscribe(
      processReading<TemperatureReading>);

  SensorProcesses::onCoverChanged.subscribe(processReading<CoverReading>);
  SensorProcesses::onShock.subscribe(processReading<ShockReading>);
  SensorProcesses::onTipped.subscribe(processReading<TippedReading>);
}
