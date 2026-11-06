#include "comms.h"
#include "ArduinoJson/Document/JsonDocument.hpp"
#include "components/ComponentManager.h"

static WiFiClient wifiClient;
static HTTPClient httpClient;
static PubSubClient client(wifiClient);

static const char *mqtt_server;
static const char *channelTopicSub;
static const char *channelTopicPub;
static char clientId[24];

const char *serverName;
const char *apiKey = "tPmW983xXyZ";

// only for debugging, delete later maybe?
bool hadWIFI = false, hadMQTT = false;

bool wifiConnected() {
  bool hasConnected = WiFi.status() != WL_CONNECTED;
  if (hadWIFI != hasConnected) {
    if (hasConnected) {
      Serial.println("wifi Connected!");
    } else {
      Serial.println("Lost Wifi Connection...");
    }
    hadWIFI = hasConnected;
  };
  return hasConnected;
}

void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD) {
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

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Trying MQTT connection ...");

    if (client.connect(clientId)) {
      Serial.println("Connecting to MQTT broker");
      client.subscribe(channelTopicSub);
    } else {
      Serial.print("Connection Error, rc=");
      Serial.print(client.state());
      Serial.println(" ... retrying in 6 seconds");
      delay(6000);
    }
  }
}

void commsInit(const char *ssid, const char *password, const char *mqtt,
               uint16_t port, const char *sub, const char *pub,
               const char *serverURL, const char *apiKey) {

  mqtt_server = strdup(mqtt);
  channelTopicSub = strdup(sub);
  channelTopicPub = strdup(pub);
  serverName = strdup(serverURL);
  apiKey = strdup(apiKey);

  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(clientId, sizeof(clientId), "%s-%02X%02X%02X%02X%02X%02X", "Client-",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  wifiInit(ssid, password);
  client.setServer(mqtt_server, port);
  client.setCallback(callback);
};

void commsUpdate() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();
};

JsonDocument getApi(const char *path) {
  JsonDocument doc;
  String url = String(serverName) + path;
  if (!httpClient.begin(wifiClient, url)) {
    Serial.print("Coulnt establish connection with: ");
    Serial.println(serverName);
    return doc;
  };

  // IMPORTANTE: Enviamos la API Key en los Headers como lo configuramos en
  // Node.js
  httpClient.addHeader("x-api-key", apiKey);

  // Realizar la petición GET
  int httpResponseCode = httpClient.GET();

  if (httpResponseCode == 200) {
    String payload = httpClient.getString();

    // Reservar memoria para el JSON (Ajustar según el tamaño del límite)
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {

      return doc;

    } else {
      Serial.print("Couldnt process JSON response: ");
      Serial.println(error.c_str());
    }
  } else {
    Serial.printf("HTTP Error: %d\n", httpResponseCode);
  }

  httpClient.end();

  return doc;
}

void commsPublish(const char *payload) {
  char msg[64];
  snprintf(msg, sizeof(msg), "%s: %s", clientId, payload);
  client.publish(channelTopicPub, msg);
};

void callback(char *topic, byte *payload, unsigned int length) {
  char *cstring = (char *)payload;
  cstring[length] = '\0'; // Adds a terminate to end of string based on length
                          // of current payload
  Serial.println(cstring);
  switch (cstring[1]) {
  case 'R':
    Serial.println("\tRojo");
    ComponentManager::led.setRed();
    break;
  case 'G':
    Serial.println("\tVerde");
    ComponentManager::led.setGreen();
    break;
  case 'B':
    Serial.println("\tAzul");
    ComponentManager::led.setBlue();
    break;
  case 'X':
    Serial.println("\tAPAGAR todo!");
    ComponentManager::led.setOff();
    break;
  default:
    break;
  }
}

void solicitarHoraServidor() {
  JsonDocument doc = getApi("/api/datetime");

  if (doc.isNull()) {
    return;
  };

  // Asignación directa a variables globales
  String fechaServidor = doc["date"].as<String>();
  int diaDeLaSemana = doc["dayNumber"].as<int>();
  String horaServidor = doc["time"].as<String>();

  Serial.println("\n--- [OPERACIÓN HORA REALIZADA] ---");
  Serial.printf("Fecha del Servidor: %s\n", fechaServidor.c_str());
  Serial.printf("Número de Día:      %d\n", diaDeLaSemana);
  Serial.printf("Hora del Servidor:  %s\n", horaServidor.c_str());
  Serial.println("-----------------------------------\n");
}
