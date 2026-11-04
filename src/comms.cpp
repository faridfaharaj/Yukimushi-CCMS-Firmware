#include "comms.h"
#include "components/ComponentManager.h"

static WiFiClient espClient;
static PubSubClient client(espClient);

static const char *mqtt_server;
static const char *channelTopicSub;
static const char *channelTopicPub;
static char clientId[24];

void wifiInit(const char *WIFI_SSID, const char *WIFI_PASSWORD) {
  delay(100);
  Serial.println();
  Serial.print("macAddress: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Seting up WiFi --> ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  short timeOutTimer = 30 * 2;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (timeOutTimer == 0) {
      Serial.print("\r      \r");
      Serial.print("Its taking too long to connect    ");
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

void reconnect() {
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

void commsInit(const char *ssid, const char *password, const char *server,
               uint16_t port, const char *sub, const char *pub) {

  mqtt_server = strdup(server);
  channelTopicSub = strdup(sub);
  channelTopicPub = strdup(pub);

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
    reconnect();
  }
  client.loop();
};

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
