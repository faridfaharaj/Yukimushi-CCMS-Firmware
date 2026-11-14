#include "HardwareSerial.h"
#include "src/CommsManager.h"
#include "src/Config.h"
#include "src/Messaging.h"
#include "src/SensorProcesses.h"
#include "src/components/ComponentRegistry.h"
#include <ESP8266HTTPClient.h>
#include <LittleFS.h>
#include <cstdio>

void setup() {
  Serial.begin(115200);
  delay(200);

  // Json Configuration
  if (!LittleFS.begin()) {
    Serial.println(
        "ERROR: Failed to mount LittleFS, Disk storage will be unavailable!");
  }

  JsonDocument doc = Config::load();

  if (doc.isNull()) {
    Serial.println("Config failed, halting");
    while (true)
      delay(1000);
  }

  const char *WIFI_SSID = doc["wifi"]["ssid"] | "";
  const char *WIFI_PASSWORD = doc["wifi"]["password"] | "";

  const char *mqttServer = doc["mqtt"]["server"] | "";
  const char *topicSub = doc["mqtt"]["topic_sub"] | "";
  uint16_t port = doc["mqtt"]["port"] | 1883;

  const char *httpServer = doc["server"]["url"] | "";
  const char *key = doc["server"]["api_key"] | "";

  const char *topicCover = doc["mqtt"]["topic_readings"]["cover"] | "";
  const char *topicHumidity = doc["mqtt"]["topic_readings"]["humidity"] | "";
  const char *topicLight = doc["mqtt"]["topic_readings"]["light"] | "";
  const char *topicTemperature =
      doc["mqtt"]["topic_readings"]["temperature"] | "";
  const char *topicTipped = doc["mqtt"]["topic_readings"]["tipped"] | "";
  const char *topicShock = doc["mqtt"]["topic_readings"]["shock"] | "";

  // init
  CommsManager::wifiInit(WIFI_SSID, WIFI_PASSWORD);
  CommsManager::mqttInit(mqttServer, port, topicSub);
  CommsManager::httpInit(httpServer);
  CommsManager::setApiKey(key);
  CommsManager::ntpInit();

  ComponentRegistry::initializeComponents();

  SensorProcesses::init();
  Messaging::init(topicCover, topicHumidity, topicLight, topicShock,
                  topicTemperature, topicTipped);

  Messaging::publishStorage();
}

void loop() {
  CommsManager::update();
  SensorProcesses::update();
}
