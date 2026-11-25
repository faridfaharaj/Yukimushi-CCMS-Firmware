#pragma once

#include "NewPing.h"
#include "custom/Accelerometer.h"
#include "custom/LDR.h"
#include "custom/Multiplexer.h"
#include <Arduino.h>
#include <DHT.h>
#include <SoftwareSerial.h>

namespace ComponentRegistry {
extern DHT tempHumSensor;
extern NewPing distanceSensor;
extern Multiplexer multiplexer;
extern Accelerometer accelerometer;
extern LDR ldr;
extern SoftwareSerial bluetooth;

void initializeComponents();
}; // namespace ComponentRegistry
