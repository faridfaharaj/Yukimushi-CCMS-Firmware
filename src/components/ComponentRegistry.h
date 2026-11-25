#pragma once

#include "custom/Accelerometer.h"
#include "custom/LDR.h"
#include "custom/Button.h"
#include "custom/DistanceSensor.h"
#include "custom/Multiplexer.h"
#include <Arduino.h>
#include <DHT.h>
#include <SoftwareSerial.h>

namespace ComponentRegistry {
extern DHT tempHumSensor;
extern DistanceSensor distanceSensor;
extern Button btn;
extern Multiplexer multiplexer;
extern Accelerometer accelerometer;
extern LDR ldr;
extern SoftwareSerial bluetooth;

void initializeComponents();
}; // namespace ComponentRegistry
