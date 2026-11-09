#pragma once
#include "containers/CoverReading.h"
#include "containers/HumidityReading.h"
#include "containers/LightReading.h"
#include "containers/ShockReading.h"
#include "containers/TemperatureReading.h"
#include "containers/TippedReading.h"

namespace InMemoryStorage {

void init();

void storeCover(bool);
void storeHumidity(float);
void storeLight(float);
void storeShock();
void storeTemperature(float);
void storeTipped(float);

} // namespace InMemoryStorage
