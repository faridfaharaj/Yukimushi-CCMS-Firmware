#include "src/CommsManager.h"
#include "src/components/ComponentManager.h"
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

  const char *server = doc["mqtt"]["server"] | "";
  const char *topicPub = doc["mqtt"]["topic_pub"] | "";
  const char *topicSub = doc["mqtt"]["topic_sub"] | "";

  const char *httpServer = doc["server"]["url"] | "";
  const char *key = doc["server"]["api_key"] | "";

  uint16_t port = doc["mqtt"]["port"] | 1883;

  CommsManager::commsInit(WIFI_SSID, WIFI_PASSWORD, server, port, topicSub,
                          topicPub, httpServer, key);

  ComponentManager::initializeComponents();
}

void loop() {
  /*
  float x = ComponentManager::accelerometer.getAccelerationX(),
        y = ComponentManager::accelerometer.getAccelerationY(),
        z = ComponentManager::accelerometer.getAccelerationZ();

  String msg1 = "x:" + String(x) + " y:" + y + " z:" + z;
  Serial.println(msg1);

  String msg2 = String(ComponentManager::ldr.readLux()) + " lux";
  Serial.println(msg2);
  */

  delay(500);
}
