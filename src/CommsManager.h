#pragma once
#include <ArduinoJson.h>
#include <stdint.h>

namespace CommsManager {

void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD);
void mqttInit(const char *server, uint16_t port, const char *sub,
              const char *pub);
void httpInit(const char *serverURL);
void setApiKey(const char *key);

bool mqttConnect();
void update();
bool wifiConnected();

bool getApi(const char *path, JsonDocument &doc);
void mqttPublish(const char *payload);

bool timeInit();

void ntpInit();

} // namespace CommsManager
