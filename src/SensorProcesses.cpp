#include "SensorProcesses.h"
#include "components/ComponentRegistry.h"

namespace SensorProcesses {
Event<4, double> onReadLight;
Event<4, float, float, float> onReadAcceleration;
Event<4, float> onReadHumidity;
Event<4, float> onReadTemperature;
Event<4, bool> onCoverChanged;

constexpr unsigned long LIGHT_RATE_MS = 300;
constexpr unsigned long ACCELERATION_RATE_MS = 300;
constexpr unsigned long TEMPERATURE_RATE_MS = 300;
constexpr unsigned long HUMIDITY_RATE_MS = 300;
constexpr unsigned long COVER_RATE_MS = 300;
constexpr unsigned long COVER_DISTANCE_CM = 3;

void readLight() {
  static unsigned long lastRead = millis() - LIGHT_RATE_MS;
  if (millis() - lastRead < LIGHT_RATE_MS)
    return;
  lastRead = millis();

  double lux = ComponentRegistry::ldr.readLux();
  onReadLight.emit(lux);
}

void readAcceleration() {
  static unsigned long lastRead = millis() - ACCELERATION_RATE_MS;
  if (millis() - lastRead < ACCELERATION_RATE_MS)
    return;
  lastRead = millis();

  float accX, accY, accZ;
  ComponentRegistry::accelerometer.readAcceleration(accX, accY, accZ);
  onReadAcceleration.emit(accX, accY, accZ);
}

void readTemperature() {
  static unsigned long lastRead = millis() - TEMPERATURE_RATE_MS;
  if (millis() - lastRead < TEMPERATURE_RATE_MS)
    return;
  lastRead = millis();

  float degrees = ComponentRegistry::tempHumSensor.readTemperature();
  onReadTemperature.emit(degrees);
}

void readHumidity() {
  static unsigned long lastRead = millis() - HUMIDITY_RATE_MS;
  if (millis() - lastRead < HUMIDITY_RATE_MS)
    return;
  lastRead = millis();

  float humidity = ComponentRegistry::tempHumSensor.readHumidity();
  onReadHumidity.emit(humidity);
}

void readCovered() {
  static unsigned long lastRead = millis() - COVER_RATE_MS;
  if (millis() - lastRead < COVER_RATE_MS)
    return;
  lastRead = millis();

  unsigned int cm = ComponentRegistry::distanceSensor.ping_cm();
  onCoverChanged.emit(cm > COVER_DISTANCE_CM);
}
} // namespace SensorProcesses

void SensorProcesses::readbtn(unsigned long time) {
  Serial.println("ButtonPressed! for: " + String(time) + "ms");
}

void SensorProcesses::init() {
  ComponentRegistry::btn.onRelease.subscribe(readbtn);
}

void SensorProcesses::update() {
  SensorProcesses::readLight();
  SensorProcesses::readAcceleration();
  SensorProcesses::readHumidity();
  SensorProcesses::readTemperature();
  SensorProcesses::readCovered();
}
