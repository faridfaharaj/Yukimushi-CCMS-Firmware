#pragma once

#include "NewPing.h"
#include "custom/Accelerometer.h"
#include "custom/Button.h"
#include "custom/LDR.h"
#include "custom/LED.h"
#include "custom/Multiplexer.h"
#include <Arduino.h>
#include <DHT.h>

namespace ComponentRegistry {
extern DHT tempHumSensor;
extern NewPing distanceSensor;
extern Button btn;
extern LED led;
extern Multiplexer multiplexer;
extern Accelerometer accelerometer;
extern LDR ldr;

void initializeComponents();
}; // namespace ComponentRegistry
