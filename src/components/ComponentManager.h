#pragma once

#include "NewPing.h"
#include "custom/Accelerometer.h"
#include "custom/LDR.h"
#include "custom/Multiplexer.h"
#include "custom/RGBLed.h"
#include <Arduino.h>
#include <DHT.h>

class ComponentManager {
public:
  static RGBLed led;
  static LDR ldr;
  /*
  static DHT tempHumSensor;
  static NewPing distanceSensor;
  static Multiplexer multiplexer;
  static Accelerometer accelerometer;
  */

  static void initializeComponents();
};
