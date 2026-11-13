#pragma once
#include "containers/CoverReading.h"
#include "containers/HumidityReading.h"
#include "containers/LightReading.h"
#include "containers/ShockReading.h"
#include "containers/TemperatureReading.h"
#include "containers/TippedReading.h"

namespace InMemoryStorage {

void init();

void store(const HumidityReading &r);
void store(const TemperatureReading &r);
void store(const LightReading &r);
void store(const CoverReading &r);
void store(const ShockReading &r);
void store(const TippedReading &r);

} // namespace InMemoryStorage
