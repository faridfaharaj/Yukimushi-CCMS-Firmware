#pragma once
#include "Event.h"

namespace SensorProcesses {

extern Event<4, double> onReadLight;
extern Event<4, float, float, float> onReadAcceleration;
extern Event<4, float> onReadHumidity;
extern Event<4, float> onReadTemperature;
extern Event<4, bool> onCoverChanged;

void readbtn(unsigned long time);

void init();

void readLight();

void readAcceleration();

void readTemperature();

void readHumidity();

void readCovered();

void update();

} // namespace SensorProcesses
