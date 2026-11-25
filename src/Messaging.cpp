#include "Messaging.h"
#include "CommsManager.h"
#include "SensorProcesses.h"
#include "storage/InMemoryStorage.h"
#include <ESP8266WiFi.h>

namespace Messaging {

const char *channelTopicGeospatial = nullptr;
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
  CommsManager::bluetoothSend("{\"type\":\"humidity\",\"value\":",
                              reading.getHumidityPercentage(), "}\n");

  return publishReadingBase(reading, channelTopicHumidity);
}
bool publishReading(TemperatureReading &reading) {
  CommsManager::bluetoothSend(
      "{\"type\":\"temperature\",\"value\":", reading.getDegrees(), "}\n");

  return publishReadingBase(reading, channelTopicTemperature);
}
bool publishReading(LightReading &reading) {
  CommsManager::bluetoothSend(
      "{\"type\":\"light\",\"value\":", reading.getLux(), "}\n");

  return publishReadingBase(reading, channelTopicLight);
}
bool publishReading(CoverReading &reading) {
  if (reading.getOpen())
    CommsManager::bluetoothSend("{\"type\":\"cover\",\"open\":true}\n");

  return publishReadingBase(reading, channelTopicCover);
}
bool publishReading(ShockReading &reading) {
  CommsManager::bluetoothSend("{\"type\":\"shock\"}\n");

  return publishReadingBase(reading, channelTopicShock);
}
bool publishReading(TippedReading &reading) {
  CommsManager::bluetoothSend("{\"type\":\"tipped\"}\n");

  return publishReadingBase(reading, channelTopicTipped);
}
bool publishReading(GeospatialReading &reading) {
  return publishReadingBase(reading, channelTopicGeospatial);
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
  publishStored<GeospatialReading>();
  publishStored<HumidityReading>();
  publishStored<TemperatureReading>();
  publishStored<LightReading>();
  publishStored<CoverReading>();
  publishStored<ShockReading>();
  publishStored<TippedReading>();
}

void fillTopic(const char *tmpl, const char *device, const char *&outChar) {
  const char *token = "{device}";
  const char *hit = strstr(tmpl, token);

  size_t len = strlen(tmpl);
  if (hit)
    len = len - strlen(token) + strlen(device);

  char *out = (char *)malloc(len + 1);
  if (!out) {
    outChar = nullptr;
    return;
  }

  if (!hit) {
    strcpy(out, tmpl);
  } else {
    size_t prefix = hit - tmpl;
    memcpy(out, tmpl, prefix);
    strcpy(out + prefix, device);
    strcpy(out + prefix + strlen(device), hit + strlen(token));
  }

  outChar = out;
}

void Messaging::init(const char *topicCover, const char *topicHumidity,
                     const char *topicLight, const char *topicShock,
                     const char *topicTemperature, const char *topicTipped,
                     const char *topicGeospatial) {

  String addr = WiFi.macAddress();
  addr.replace(":", "");
  const char *charMac = addr.c_str();

  fillTopic(topicGeospatial, charMac, channelTopicCover);
  fillTopic(topicCover, charMac, channelTopicCover);
  fillTopic(topicHumidity, charMac, channelTopicHumidity);
  fillTopic(topicLight, charMac, channelTopicLight);
  fillTopic(topicShock, charMac, channelTopicShock);
  fillTopic(topicTemperature, charMac, channelTopicTemperature);
  fillTopic(topicTipped, charMac, channelTopicTipped);

  CommsManager::onMQTTconnected.subscribe(publishStorage);
  SensorProcesses::onReadGPS.subscribe(processReading<GeospatialReading>);
  SensorProcesses::onReadHumidity.subscribe(processReading<HumidityReading>);
  SensorProcesses::onReadLight.subscribe(processReading<LightReading>);
  SensorProcesses::onReadTemperature.subscribe(
      processReading<TemperatureReading>);

  SensorProcesses::onCoverChanged.subscribe(processReading<CoverReading>);
  SensorProcesses::onShock.subscribe(processReading<ShockReading>);
  SensorProcesses::onTipped.subscribe(processReading<TippedReading>);
}
