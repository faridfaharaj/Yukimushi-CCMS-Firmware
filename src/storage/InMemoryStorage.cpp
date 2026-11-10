#include "InMemoryStorage.h"
#include "../SensorProcesses.h"
#include "containers/CoverReading.h"
#include "containers/HumidityReading.h"
#include "containers/LightReading.h"
#include "containers/Queue.h"
#include "containers/ShockReading.h"
#include "containers/TemperatureReading.h"
#include "containers/TippedReading.h"

namespace InMemoryStorage {

Queue<416, HumidityReading> Humidity;
Queue<416, TemperatureReading> temperature;
Queue<416, LightReading> light;

Queue<666, CoverReading> cover;

Queue<833, ShockReading> shock;
Queue<833, TippedReading> Tipped;

void storeCover(bool reading) {
  if (cover.isFull())
    return;

  CoverReading coverR(reading);
  cover.push(coverR);
}

void storeHumidity(float reading) {
  if (Humidity.isFull())
    return;

  HumidityReading humidityR(reading);
  Humidity.push(humidityR);
}

void storeLight(float reading) {
  if (light.isFull())
    return;

  LightReading lightR(reading);
  light.push(lightR);
}

void storeShock() {
  if (shock.isFull())
    return;

  ShockReading shockR;
  shock.push(shockR);
}

void storeTemperature(float reading) {
  if (temperature.isFull())
    return;

  TemperatureReading temperatureR(reading);
  temperature.push(temperatureR);
}

void storeTipped(float reading) {
  if (Tipped.isFull())
    return;

  TippedReading tippedR;
  Tipped.push(tippedR);
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
