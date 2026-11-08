#pragma once

#include "NewPing.h"
#include "custom/Accelerometer.h"
#include "custom/Button.h"
#include "custom/LDR.h"
#include "custom/LED.h"
#include "custom/Multiplexer.h"
#include <Arduino.h>
#include <DHT.h>

class ComponentManager {
public:
  static DHT tempHumSensor;
  static NewPing distanceSensor;
  static Button btn;
  static LED led;
  static Multiplexer multiplexer;
  static Accelerometer accelerometer;
  static LDR ldr;

  static void initializeComponents();
};
