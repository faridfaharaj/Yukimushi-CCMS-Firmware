#include "src/comms.h"
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

  // wifi
  const char *WIFI_SSID = doc["wifi"]["ssid"] | "";
  const char *WIFI_PASSWORD = doc["wifi"]["password"] | "";

  const char *server = doc["mqtt"]["server"] | "";
  const char *topicPub = doc["mqtt"]["topic_pub"] | "";
  const char *topicSub = doc["mqtt"]["topic_sub"] | "";

  uint16_t port = doc["mqtt"]["port"] | 1883;

  commsInit(WIFI_SSID, WIFI_PASSWORD, server, port, topicSub, topicPub);

  ComponentManager::initializeComponents();
}

// const char* serverName = "http://10.22.11.98:3000/api/sensor";
// const char* urlObtenerHora = "http://10.22.11.98:3000/api/datetime";
const char *serverName = "http://10.22.188.209:3049/api/sensor";
const char *urlObtenerHora = "http://10.22.188.209:3049/api/datetime";
const char *apiKey = "tPmW983xXyZ";

// --- VARIABLES GLOBALES DE TIEMPO ---
String fechaServidor = "";
String horaServidor = "";
int diaDeLaSemana = -1; // Almacenará el número del día devuelto

int hora = 0;
int minuto = 0;
int segundo = 0;
void loop() {
  /*
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);

      // IMPORTANTE: Enviamos la API Key en los Headers como lo configuramos en
      // Node.js
      http.addHeader("x-api-key", apiKey);

      // Realizar la petición GET
      int httpResponseCode = http.GET();

      if (httpResponseCode == 200) {
        String payload = http.getString();
        Serial.println("\nDatos recibidos del servidor:");

        // Reservar memoria para el JSON (Ajustar según el tamaño del límite)
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
          // Como el API devuelve una lista [], iteramos sobre ella
          for (JsonObject item : doc.as<JsonArray>()) {
            int id = item["id"];
            const char *reading = item["reading"];
            const char *fecha = item["reg_date"];

            Serial.printf("ID: %d | Lectura: %s | Fecha: %s\n", id, reading,
                          fecha);
          }
        } else {
          Serial.print("Error al procesar JSON: ");
          Serial.println(error.c_str());
        }
      } else {
        Serial.printf("Error en HTTP: %d\n", httpResponseCode);
      }
      http.end();
    }
  */

  /*
  float x = ComponentManager::accelerometer.getAccelerationX(),
        y = ComponentManager::accelerometer.getAccelerationY(),
        z = ComponentManager::accelerometer.getAccelerationZ();

  String msg = "x:" + String(x) + " y:" + y + " z:" + z;
  */

  String msg = String(ComponentManager::ldr.readLux()) + " lux";
  Serial.println(msg);

  delay(500); // Consultar cada 15 segundos
}

void solicitarHoraServidor() {

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, urlObtenerHora);

    // Inyección de API key a nivel de encabezado HTTP
    http.addHeader("x-api-key", apiKey);

    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String response = http.getString();

      // Parsear la estructura JSON específica recibida
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, response);

      if (!error) {
        // Asignación directa a variables globales
        fechaServidor = doc["date"].as<String>();
        diaDeLaSemana = doc["dayNumber"].as<int>();
        horaServidor = doc["time"].as<String>();

        Serial.println("\n--- [OPERACIÓN HORA REALIZADA] ---");
        Serial.printf("Fecha del Servidor: %s\n", fechaServidor.c_str());
        Serial.printf("Número de Día:      %d\n", diaDeLaSemana);
        Serial.printf("Hora del Servidor:  %s\n", horaServidor.c_str());
        Serial.println("-----------------------------------\n");
      } else {
        Serial.print("Error procesando formato de tiempo JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.printf("[OPERACIÓN HORA] Falló con código HTTP: %d\n",
                    httpResponseCode);
    }
    http.end();
  }
}
