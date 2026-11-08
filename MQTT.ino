#include "src/CommsManager.h"
#include "src/components/ComponentRegistry.h"
#include "src/config.h"
#include <ESP8266HTTPClient.h>
#include <cstdio>

void setup() {
  Serial.begin(115200);
  delay(200);

  // Json Configuration
  JsonDocument doc = getConfig();

  if (doc.isNull()) {
    Serial.println("Config failed, halting");
    while (true)
      delay(1000);
  }

  const char *WIFI_SSID = doc["wifi"]["ssid"] | "";
  const char *WIFI_PASSWORD = doc["wifi"]["password"] | "";

  const char *mqttServer = doc["mqtt"]["server"] | "";
  const char *topicPub = doc["mqtt"]["topic_pub"] | "";
  const char *topicSub = doc["mqtt"]["topic_sub"] | "";
  uint16_t port = doc["mqtt"]["port"] | 1883;

  const char *httpServer = doc["server"]["url"] | "";
  const char *key = doc["server"]["api_key"] | "";

  // init
  CommsManager::wifiInit(WIFI_SSID, WIFI_PASSWORD);
  CommsManager::mqttInit(mqttServer, port, topicPub, topicSub);
  CommsManager::httpInit(httpServer);
  CommsManager::setApiKey(key);

  ComponentRegistry::initializeComponents();
}

void loop() {
  CommsManager::update();
  delay(500);
}
