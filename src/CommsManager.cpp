#include "CommsManager.h"
#include "HardwareSerial.h"
#include "core_esp8266_features.h"
#include "util.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <WiFiClientSecure.h>

namespace CommsManager {

BearSSL::WiFiClientSecure mqttWifiSecure;
PubSubClient mqttClient(mqttWifiSecure);
BearSSL::X509List *caCert;
BearSSL::X509List *clientCert;
BearSSL::PrivateKey *clientKey;

constexpr size_t TOPIC_MAX_LENGTH = 95;

const char *mqttServer = nullptr;
char clientId[24];
const char *channelTopicTemplate = nullptr;

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

  if (time(nullptr) < 1700000000)
    return false;

  Serial.println("Trying to connect mqtt...");

  mqttWifiSecure.stop();
  if (!mqttClient.connect(clientId)) {
    char err[128];
    int code = mqttWifiSecure.getLastSSLError(err, sizeof(err));
    Serial.printf("ssl code=%d msg=%s\n", code, err);
    return false;
  }

  mqttClient.subscribe(getTopic(MqttTopicMessageType::NCMD));
  onMQTTconnected.emit();
  return true;
}

bool getTopicTemplate(const char *org, const char *node, const char *&outChar) {

  static char topic[TOPIC_MAX_LENGTH + 1];
  int n = snprintf(topic, sizeof(topic), "yukimushi/%s/%s/%s", org, "%s", node);

  if (n < 0 || n > TOPIC_MAX_LENGTH) {
    outChar = nullptr;
    return false;
  }

  outChar = topic;
  return true;
}

} // namespace CommsManager

const char *CommsManager::getTopic(MqttTopicMessageType type) {
  static char topic[TOPIC_MAX_LENGTH + 1];

  if (!channelTopicTemplate)
    return nullptr;

  int n = snprintf(topic, sizeof(topic), channelTopicTemplate, toString(type));
  if (n < 0 || n > TOPIC_MAX_LENGTH) {
    return nullptr;
  };

  return topic;
}

void CommsManager::wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD) {

  onWifiConnected =
      WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP &) {
        Serial.println("wifi Connected!");
      });
  onWifiDisconnected = WiFi.onStationModeDisconnected(
      [](const WiFiEventStationModeDisconnected &) {
        Serial.println("Wifi Connection lost, Reconnecting...");
      });

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
      Serial.print("\b\b\b\b    \b\b\b\b");
    }

    timeOutTimer--;
  }
  Serial.println();
  Serial.println("WiFi connnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void CommsManager::mqttInit(const char *server, uint16_t port,
                            const char *org) {
  mqttServer = strdup(server);

  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(clientId, sizeof(clientId), "%02X%02X%02X%02X%02X%02X", mac[0],
           mac[1], mac[2], mac[3], mac[4], mac[5]);

  {
    String s = readFile("/ca.crt");
    caCert = new BearSSL::X509List(s.c_str());
  }
  {
    String s = readFile("/client.crt");
    clientCert = new BearSSL::X509List(s.c_str());
  }
  {
    String s = readFile("/client.key");
    clientKey = new BearSSL::PrivateKey(s.c_str());
  }

  mqttWifiSecure.setTrustAnchors(caCert);
  mqttWifiSecure.setClientECCert(clientCert, clientKey, BR_KEYTYPE_SIGN, 0);
  mqttWifiSecure.setBufferSizes(1024, 512);
  mqttWifiSecure.setTimeout(10000);

  getTopicTemplate(org, clientId, channelTopicTemplate);

  mqttClient.setServer(mqttServer, port);
  mqttClient.setCallback(callback);
  mqttClient.setSocketTimeout(10);
}

void CommsManager::ntpInit() { configTime(0, 0, "pool.ntp.org"); }

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

bool CommsManager::mqttPublish(const char *topic, const uint8_t *payload,
                               size_t len) {
  return mqttClient.publish(topic, payload, len);
}
