#include "Messaging.h"
#include "CommsManager.h"
#include "SensorProcesses.h"
#include "storage/InMemoryStorage.h"
#include "util.h"
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include <cstdio>

namespace Messaging {

constexpr unsigned long STATUS_UPDATE_RATE_MS = 60000;

WiFiEventHandler onConnected;

template <typename T> bool publishReadingBase(T &reading) {
  uint8_t buffer[reading.size()];
  size_t n = reading.serialize(buffer);

  return CommsManager::wifiConnected() && CommsManager::mqttConnected() &&
         CommsManager::mqttPublish(
             CommsManager::getTopic(MqttTopicMessageType::NDATA), buffer, n);
}

bool publishReading(HumidityReading &reading) {
  CommsManager::bluetoothSend("{\"type\":\"humidity\",\"value\":",
                              reading.getHumidityPercentage(), "}\n");

  return publishReadingBase(reading);
}
bool publishReading(TemperatureReading &reading) {
  CommsManager::bluetoothSend(
      "{\"type\":\"temperature\",\"value\":", reading.getDegrees(), "}\n");

  return publishReadingBase(reading);
}
bool publishReading(LightReading &reading) {
  CommsManager::bluetoothSend(
      "{\"type\":\"light\",\"value\":", reading.getLux(), "}\n");

  return publishReadingBase(reading);
}
bool publishReading(CoverReading &reading) {
  if (reading.getOpen())
    CommsManager::bluetoothSend("{\"type\":\"cover\",\"open\":true}\n");

  return publishReadingBase(reading);
}
bool publishReading(ShockReading &reading) {
  CommsManager::bluetoothSend("{\"type\":\"shock\"}\n");

  return publishReadingBase(reading);
}
bool publishReading(TippedReading &reading) {
  CommsManager::bluetoothSend("{\"type\":\"tipped\"}\n");

  return publishReadingBase(reading);
}
bool publishReading(GeospatialReading &reading) {
  return publishReadingBase(reading);
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

void sendStatusUpdate(DeviceState state) {
  uint8_t payload = static_cast<uint8_t>(state);
  CommsManager::mqttPublish(CommsManager::getTopic(MqttTopicMessageType::DDATA),
                            &payload, sizeof(payload));
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

void Messaging::init() {

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

void Messaging::update() {

  static unsigned long last = 0;
  if (due(last, STATUS_UPDATE_RATE_MS)) {
    sendStatusUpdate(DeviceState::ONLINE);
  }
}
