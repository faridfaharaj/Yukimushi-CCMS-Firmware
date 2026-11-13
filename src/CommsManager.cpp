#include "CommsManager.h"
#include "core_esp8266_features.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

namespace CommsManager {

WiFiClient mqttWifiClient{};
PubSubClient mqttClient{mqttWifiClient};

WiFiClient httpWifiClient{};
HTTPClient httpClient{};

const char *mqttServer = nullptr;
const char *channelTopicSub = nullptr;
char clientId[24];

const char *serverName = nullptr;
const char *apiKey = nullptr;

WiFiEventHandler onWifiConnected;
WiFiEventHandler onWifiDisconnected;

Event<1> onMQTTconnected;

// only for debugging, delete later maybe?
bool hadMQTT = false;

void callback(char *topic, byte *payload, unsigned int length) {
  char cstring[64];
  size_t n = min((size_t)length, sizeof(cstring) - 1);
  memcpy(cstring, payload, n);
  cstring[n] = '\0';

  // do stuff
}

bool mqttConnect() {
  static unsigned long lastTry = millis() - 5000;
  if (millis() - lastTry < 5000)
    return false;
  lastTry = millis();

  mqttWifiClient.stop();
  if (!mqttClient.connect(clientId))
    return false;
  mqttClient.subscribe(channelTopicSub);
  onMQTTconnected.emit();
  return true;
}

} // namespace CommsManager

void CommsManager::wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD) {

  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(clientId, sizeof(clientId), "%02X%02X%02X%02X%02X%02X", mac[0],
           mac[1], mac[2], mac[3], mac[4], mac[5]);

  delay(100);
  Serial.println();
  Serial.print("macAddress: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Seting up WiFi --> ");
  Serial.print(WIFI_SSID);
  Serial.print("   ");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  short timeOutTimer = 30 * 2;
  while (!wifiConnected()) {
    delay(500);
    if (timeOutTimer == 0) {
      Serial.print("\r      \r");
      Serial.println("Too too long");
      Serial.println("Continuing without connection...");
      return;
    }

    Serial.print(".");

    if (timeOutTimer % 4 == 0) {
      timeOutTimer = timeOutTimer < 0 ? 0 : timeOutTimer;
      Serial.print("\b\b\b\b    \b\b\b\b");
    }

    timeOutTimer--;
  }
  Serial.println();
  Serial.println("WiFi connnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  onWifiConnected =
      WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP &) {
        Serial.println("wifi Connected!");
      });
  onWifiDisconnected = WiFi.onStationModeDisconnected(
      [](const WiFiEventStationModeDisconnected &) {
        Serial.println("Wifi Connection lost, Reconnecting...");
      });
}

void CommsManager::mqttInit(const char *server, uint16_t port,
                            const char *sub) {
  mqttServer = strdup(server);
  channelTopicSub = strdup(sub);

  mqttClient.setServer(mqttServer, port);
  mqttClient.setCallback(callback);
  mqttClient.setSocketTimeout(2);

  if (!wifiConnected())
    return;
  Serial.println("Trying MQTT connection...");
  if (mqttConnect()) {
    Serial.println("Connected to MQTT broker!");
    hadMQTT = true;
  } else {
    Serial.print("Connection Error, rc=");
    Serial.println(mqttClient.state());
  }
}

void CommsManager::httpInit(const char *serverURL) {
  httpClient.setTimeout(2000);
  serverName = strdup(serverURL);
}

void CommsManager::ntpInit() { configTime(0, 0, "pool.ntp.org"); }

void CommsManager::setApiKey(const char *key) { apiKey = strdup(key); }

void CommsManager::update() {
  bool hasMQTT = CommsManager::mqttConnected();
  if (!hasMQTT && wifiConnected()) {
    hasMQTT = mqttConnect();
  }
  if (hasMQTT != hadMQTT) {
    if (hasMQTT) {
      Serial.println("MQTT broker connected!");
    } else {
      Serial.println("MQTT broker disconected!");
    }
    hadMQTT = hasMQTT;
  }

  mqttClient.loop();
}

bool CommsManager::wifiConnected() { return WiFi.status() == WL_CONNECTED; }
bool CommsManager::mqttConnected() { return mqttClient.connected(); }

bool CommsManager::getApi(const char *path, JsonDocument &doc) {
  String url = String(serverName) + path;

  if (!httpClient.begin(httpWifiClient, url)) {
    Serial.print("Couldn't establish connection with: ");
    Serial.println(serverName);
    return false;
  }

  httpClient.addHeader("x-api-key", apiKey);

  int httpResponseCode = httpClient.GET();
  bool ok = false;

  if (httpResponseCode == 200) {
    DeserializationError error = deserializeJson(doc, httpClient.getStream());
    if (error) {
      Serial.print("Couldn't process JSON response: ");
      Serial.println(error.c_str());
    } else {
      ok = true;
    }
  } else {
    Serial.printf("HTTP Error: %d\n", httpResponseCode);
  }

  httpClient.end();
  return ok;
}

bool CommsManager::mqttPublish(const char *topic, const uint8_t *payload,
                               size_t len) {
  return mqttClient.publish(topic, payload, len);
}
