#pragma once
#include "components/ComponentManager.h"
#include "wl_definitions.h"
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class CommsManager {

private:
  static inline WiFiClient wifiClient{};
  static inline HTTPClient httpClient{};
  static inline PubSubClient mqttClient{wifiClient};

  static inline const char *mqtt_server = nullptr;
  static inline const char *channelTopicSub = nullptr;
  static inline const char *channelTopicPub = nullptr;
  static inline char clientId[24];

  static inline const char *serverName = nullptr;
  static inline const char *apiKey = nullptr;

  // only for debugging, delete later maybe?
  static inline bool hadWIFI = false, hadMQTT = false;

public:
  static WiFiClient getWifiClient() { return wifiClient; }

  static bool wifiConnected() {
    bool hasConnected = WiFi.status() == WL_CONNECTED;
    if (hadWIFI != hasConnected) {
      if (hasConnected) {
        Serial.println("wifi Connected");
      } else {
        Serial.println("Lost Wifi Connection...");
      }
      hadWIFI = hasConnected;
    };
    return hasConnected;
  }

  static void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD) {
    delay(100);
    Serial.println();
    Serial.print("macAddress: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Seting up WiFi --> ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    short timeOutTimer = 30 * 2;
    while (wifiConnected()) {
      delay(500);
      if (timeOutTimer == 0) {
        Serial.print("\r      \r");
        Serial.println("Too too long");
        Serial.println("Continuing without connection...");
        break;
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

  static void connectMQTT() {
    while (!mqttClient.connected()) {
      Serial.print("Trying MQTT connection ...");

      if (mqttClient.connect(clientId)) {
        Serial.println("Connecting to MQTT broker");
        mqttClient.subscribe(channelTopicSub);
      } else {
        Serial.print("Connection Error, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" ... retrying in 6 seconds");
        delay(6000);
      }
    }
  }

  static void commsInit(const char *ssid, const char *password,
                        const char *mqtt, uint16_t port, const char *sub,
                        const char *pub, const char *serverURL,
                        const char *key) {

    mqtt_server = strdup(mqtt);
    channelTopicSub = strdup(sub);
    channelTopicPub = strdup(pub);
    serverName = strdup(serverURL);
    apiKey = strdup(key);

    uint8_t mac[6];
    WiFi.macAddress(mac);
    snprintf(clientId, sizeof(clientId), "%s-%02X%02X%02X%02X%02X%02X",
             "Client-", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    wifiInit(ssid, password);
    mqttClient.setServer(mqtt_server, port);
    mqttClient.setCallback(callback);
  };

  void commsUpdate() {
    bool hasMQTT = mqttClient.connected();
    if (!hasMQTT) {
      if (hasMQTT != hadMQTT) {
        Serial.println("MQTT broker connected");
      } else {
        Serial.println("MQTT broker disconected");
      }
      connectMQTT();
    }
    hadMQTT = hasMQTT;

    mqttClient.loop();
  };

  static bool getApi(const char *path, JsonDocument &doc) {
    String url = String(serverName) + path;

    if (!httpClient.begin(wifiClient, url)) {
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

  void commsPublish(const char *payload) {
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
  };

  static void callback(char *topic, byte *payload, unsigned int length) {
    char cstring[64];
    size_t n = min((size_t)length, sizeof(cstring) - 1);
    memcpy(cstring, payload, n);
    cstring[n] = '\0';

    Serial.println(cstring);

    if (n < 2)
      return;
  }

  static void solicitarHoraServidor() {
    JsonDocument doc;

    if (getApi("/api/datetime", doc)) {
      return;
    };

    String fechaServidor = doc["date"].as<String>();
    int diaDeLaSemana = doc["dayNumber"].as<int>();
    String horaServidor = doc["time"].as<String>();

    Serial.println("\n--- [OPERACIÓN HORA REALIZADA] ---");
    Serial.printf("Fecha del Servidor: %s\n", fechaServidor.c_str());
    Serial.printf("Número de Día:      %d\n", diaDeLaSemana);
    Serial.printf("Hora del Servidor:  %s\n", horaServidor.c_str());
    Serial.println("-----------------------------------\n");
  }
};
