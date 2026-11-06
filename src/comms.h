#pragma once
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD);
void connectMQTT();

void commsInit(const char *ssid, const char *password, const char *server,
               uint16_t port, const char *sub, const char *pub, const char *serverURL, const char *apiKey);
void commsUpdate();
void commsPublish(const char *payload);

bool wifiConnected();

void callback(char *topic, byte *payload, unsigned int length);

void solicitarHoraServidor();
