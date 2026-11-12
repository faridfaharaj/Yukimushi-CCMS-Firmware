#include "Config.h"
#include <LittleFS.h>

JsonDocument Config::load() {
  JsonDocument doc;

  File file = LittleFS.open("/config.json", "r");

  if (!file) {
    Serial.println("ERROR: Failed to open /config.json");
    return doc;
  }

  DeserializationError error = deserializeJson(doc, file);

  file.close();

  if (error) {
    Serial.print("ERROR: ");
    Serial.println(error.c_str());
    doc.clear();
  }

  return doc;
}
