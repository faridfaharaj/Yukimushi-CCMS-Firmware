#include "HardwareSerial.h"
#include "src/CommsManager.h"
#include "src/Config.h"
#include "src/Messaging.h"
#include "src/SensorProcesses.h"
#include "src/components/ComponentRegistry.h"
#include "src/storage/InMemoryStorage.h"
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
  const char *topicPub = doc["mqtt"]["topic_pub"] | "";
  const char *topicSub = doc["mqtt"]["topic_sub"] | "";
  uint16_t port = doc["mqtt"]["port"] | 1883;

  const char *httpServer = doc["server"]["url"] | "";
  const char *key = doc["server"]["api_key"] | "";

  // init
  CommsManager::wifiInit(WIFI_SSID, WIFI_PASSWORD);
  CommsManager::mqttInit(mqttServer, port, topicSub, topicPub);
  CommsManager::httpInit(httpServer);
  CommsManager::setApiKey(key);
  CommsManager::ntpInit();

  ComponentRegistry::initializeComponents();

  SensorProcesses::init();
<<<<<<< HEAD
  Messaging::init();
=======
  InMemoryStorage::init();

>>>>>>> 582b752 (added writing readings to file)
}

void loop() {
  CommsManager::update();
  SensorProcesses::update();
}
