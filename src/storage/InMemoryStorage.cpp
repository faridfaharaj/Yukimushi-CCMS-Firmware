#include "InMemoryStorage.h"
#include "containers/CoverReading.h"
#include "containers/HumidityReading.h"
#include "containers/LightReading.h"
#include "containers/ShockReading.h"
#include "containers/TemperatureReading.h"
#include "containers/TippedReading.h"

namespace InMemoryStorage {

HumidityReading Humidity[416];
TemperatureReading temperature[416];
LightReading light[416];

CoverReading cover[666];

ShockReading shock[833];
TippedReading Tipped[833];

} // namespace InMemoryStorage

void InMemoryStorage::init() {}
