#pragma once
#include "DeviceState.h"
#include "Event.h"
#include "components/ComponentRegistry.h"
#include <ArduinoJson.h>
#include <stdint.h>

namespace CommsManager {

extern Event<1> onMQTTconnected;

void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD);
void mqttInit(const char *server, uint16_t port, const char *org);

void update();
bool wifiConnected();
bool mqttConnected();

bool mqttPublish(const char *topic, const uint8_t *payload, size_t size);

template <typename... Args> void bluetoothSend(Args &&...args) {
  (ComponentRegistry::bluetooth.print(std::forward<Args>(args)), ...);

  ComponentRegistry::bluetooth.println();
}

bool timeInit();

void ntpInit();

const char *getTopic(MqttTopicMessageType type);

} // namespace CommsManager
