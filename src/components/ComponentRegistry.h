#pragma once

#include "custom/Accelerometer.h"
#include "custom/GPS.h"
#include "custom/LDR.h"
#include "custom/Button.h"
#include "custom/DistanceSensor.h"
#include "custom/Multiplexer.h"
#include <Arduino.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <NewPing.h>

namespace ComponentRegistry {
extern DHT tempHumSensor;
extern NewPing distanceSensor;
extern Multiplexer multiplexer;
extern Accelerometer accelerometer;
extern LDR ldr;
extern SoftwareSerial bluetooth;
extern GPS gps;

void initializeComponents();
}; // namespace ComponentRegistry
