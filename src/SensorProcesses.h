#pragma once
#include "Event.h"
#include "storage/containers/CoverReading.h"
#include "storage/containers/HumidityReading.h"
#include "storage/containers/LightReading.h"
#include "storage/containers/ShockReading.h"
#include "storage/containers/TemperatureReading.h"
#include "storage/containers/TippedReading.h"

namespace SensorProcesses {

extern Event<4, LightReading &> onReadLight;
extern Event<4, HumidityReading &> onReadHumidity;
extern Event<4, TemperatureReading &> onReadTemperature;

extern Event<4, TippedReading &> onTipped;
extern Event<4, ShockReading &> onShock;
extern Event<4, CoverReading &> onCoverChanged;

void readbtn(unsigned long time);

void init();

void readLight();
void readAcceleration();
void readTemperature();
void readHumidity();
void readCovered();

void update();

} // namespace SensorProcesses
