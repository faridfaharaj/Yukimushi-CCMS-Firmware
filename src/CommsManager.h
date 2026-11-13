#pragma once
#include "Event.h"
#include <ArduinoJson.h>
#include <stdint.h>

namespace CommsManager {

extern Event<1> onMQTTconnected;

void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD);
void mqttInit(const char *server, uint16_t port, const char *sub);
void httpInit(const char *serverURL);
void setApiKey(const char *key);

void update();
bool wifiConnected();
bool mqttConnected();

bool getApi(const char *path, JsonDocument &doc);
bool mqttPublish(const char *topic, const uint8_t *payload, size_t size);

bool timeInit();

void ntpInit();

} // namespace CommsManager
