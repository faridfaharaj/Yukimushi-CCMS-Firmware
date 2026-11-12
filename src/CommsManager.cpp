#include "CommsManager.h"
#include "core_esp8266_features.h"

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

namespace {

WiFiClient mqttWifiClient{};
PubSubClient mqttClient{mqttWifiClient};

WiFiClient httpWifiClient{};
HTTPClient httpClient{};

const char *mqttServer = nullptr;
const char *channelTopicSub = nullptr;
const char *channelTopicPub = nullptr;
char clientId[24];

const char *serverName = nullptr;
const char *apiKey = nullptr;

WiFiEventHandler onConnected;
WiFiEventHandler onDisconnected;

// only for debugging, delete later maybe?
bool hadMQTT = false;

void callback(char *topic, byte *payload, unsigned int length) {
  char cstring[64];
  size_t n = min((size_t)length, sizeof(cstring) - 1);
  memcpy(cstring, payload, n);
  cstring[n] = '\0';

  // do stuff
}

} // namespace

void CommsManager::wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD) {
  onConnected = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP &) {
    Serial.println("wifi Connected!");
  });
  onDisconnected = WiFi.onStationModeDisconnected(
      [](const WiFiEventStationModeDisconnected &) {
        Serial.println("Wifi Connection lost!");
      });

  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(clientId, sizeof(clientId), "%s-%02X%02X%02X%02X%02X%02X", "Client",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

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
}

void CommsManager::mqttInit(const char *server, uint16_t port, const char *sub,
                            const char *pub) {
  mqttServer = strdup(server);
  channelTopicSub = strdup(sub);
  channelTopicPub = strdup(pub);

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

void CommsManager::setApiKey(const char *key) { apiKey = strdup(key); }

bool CommsManager::mqttConnect() {
  static unsigned long lastTry = millis() - 5000;
  if (millis() - lastTry < 5000)
    return false;
  lastTry = millis();

  mqttWifiClient.stop();
  if (!mqttClient.connect(clientId))
    return false;
  mqttClient.subscribe(channelTopicSub);
  return true;
}

void CommsManager::update() {
  bool hasMQTT = mqttClient.connected();
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

void CommsManager::mqttPublish(const char *payload) {
  char msg[64];

  int n = snprintf(msg, sizeof(msg), "%s: %s", clientId, payload);
  if (n < 0 || n >= (int)sizeof(msg)) {
    Serial.println("Message " + String(payload) +
                   " exeeds size limit: " + sizeof(msg));
    return;
  }

  if (!mqttClient.publish(channelTopicPub, msg)) {
    Serial.println("Failed to publish: " + String(payload));
  }
}

void CommsManager::ntpInit() { configTime(0, 0, "pool.ntp.org"); }
