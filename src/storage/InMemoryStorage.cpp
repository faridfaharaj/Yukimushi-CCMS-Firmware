#include "InMemoryStorage.h"
#include "../SensorProcesses.h"
#include "DiskStorage.h"
#include "containers/CoverReading.h"
#include "containers/HumidityReading.h"
#include "containers/LightReading.h"
#include "containers/Queue.h"
#include "containers/ShockReading.h"
#include "containers/TemperatureReading.h"
#include "containers/TippedReading.h"

namespace InMemoryStorage {

Queue<416, HumidityReading> humidity;
Queue<416, TemperatureReading> temperature;
Queue<416, LightReading> light;

Queue<666, CoverReading> cover;

Queue<833, ShockReading> shock;
Queue<833, TippedReading> Tipped;

void storeCover(CoverReading coverR) {
  if (cover.isFull()) {
    if (!DiskStorage::write<666, CoverReading>(cover))
      cover.pop();
  }

  cover.push(coverR);
}

void storeHumidity(HumidityReading humidityR) {
  if (humidity.isFull()) {
    if (!DiskStorage::write<416, HumidityReading>(humidity))
      humidity.pop();
  }

  humidity.push(humidityR);
}

void storeLight(LightReading lightR) {
  if (light.isFull()) {
    if (!DiskStorage::write<416, LightReading>(light))
      light.pop();
  }

  light.push(lightR);
}

void storeShock(ShockReading shockR) {
  if (shock.isFull()) {
    if (!DiskStorage::write<833, ShockReading>(shock))
      shock.pop();
  }

  shock.push(shockR);
}

void storeTemperature(TemperatureReading temperatureR) {
  if (temperature.isFull()) {
    if (!DiskStorage::write<416, TemperatureReading>(temperature))
      temperature.pop();
  }

  temperature.push(temperatureR);
}

void storeTipped(TippedReading tippedR) {
  if (Tipped.isFull()) {
    if (!DiskStorage::write<833, TippedReading>(Tipped))
      Tipped.pop();
  }

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
