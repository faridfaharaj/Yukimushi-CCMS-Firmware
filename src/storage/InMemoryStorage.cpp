#include "InMemoryStorage.h"
#include "../SensorProcesses.h"
#include "containers/Deque.h"

namespace InMemoryStorage {

Deque<416, HumidityReading> Humidity;
Deque<416, TemperatureReading> temperature;
Deque<416, LightReading> light;

Deque<666, CoverReading> cover;

Deque<833, ShockReading> shock;
Deque<833, TippedReading> Tipped;

void storeCover(bool reading) {
  if (cover.isFull())
    return;

  CoverReading coverR(reading);
  cover.pushLast(coverR);
}

void storeHumidity(float reading) {
  if (Humidity.isFull())
    return;

  HumidityReading humidityR(reading);
  Humidity.pushLast(humidityR);
}

void storeLight(float reading) {
  if (light.isFull())
    return;

  LightReading lightR(reading);
  light.pushLast(lightR);
}

void storeShock() {
  if (shock.isFull())
    return;

  ShockReading shockR;
  shock.pushLast(shockR);
}

void storeTemperature(float reading) {
  if (temperature.isFull())
    return;

  TemperatureReading temperatureR(reading);
  temperature.pushLast(temperatureR);
}

void storeTipped(float reading) {
  if (Tipped.isFull())
    return;

  TippedReading tippedR;
  Tipped.pushLast(tippedR);
}

} // namespace InMemoryStorage

void InMemoryStorage::init() {
  SensorProcesses::onReadHumidity.subscribe(storeHumidity);
  SensorProcesses::onReadLight.subscribe(storeLight);
  SensorProcesses::onReadTemperature.subscribe(storeTemperature);

  SensorProcesses::onCoverChanged.subscribe(storeCover);
  SensorProcesses::onShock.subscribe(storeShock);
  SensorProcesses::onTipped.subscribe(storeTipped);
}
