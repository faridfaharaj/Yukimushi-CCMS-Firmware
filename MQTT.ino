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

  JsonDocument doc = Configuration::load();

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
  const char *topicGeospatial =
      doc["mqtt"]["topic_readings"]["geospatial"] | "";

  bool humidityNTemperatureEnabled =
      doc["components"]["humidityNTemperature"]["enable"] | true;
  bool accelerationEnabled = doc["components"]["acceleration"]["enable"] | true;
  bool lightEnabled = doc["components"]["light"]["enable"] | true;
  bool proximityEnabled = doc["components"]["proximity"]["enable"] | true;
  bool geospatialEnabled = doc["components"]["geospatial"]["enable"] | true;
  bool bluetoothEnabled = doc["components"]["bluetooth"]["enable"] | true;

  // init
  CommsManager::wifiInit(WIFI_SSID, WIFI_PASSWORD);
  CommsManager::mqttInit(mqttServer, port, topicSub);
  CommsManager::httpInit(httpServer);
  CommsManager::setApiKey(key);
  CommsManager::ntpInit();

  ComponentRegistry::initializeComponents();

  SensorProcesses::init(humidityNTemperatureEnabled, accelerationEnabled,
                        lightEnabled, proximityEnabled, geospatialEnabled);
  Messaging::init(topicCover, topicHumidity, topicLight, topicShock,
                  topicTemperature, topicTipped, topicGeospatial);

  Messaging::publishStorage();
}

void loop() {
  CommsManager::update();
  SensorProcesses::update();
}
