#pragma once

#include "NewPing.h"
#include "custom/Accelerometer.h"
#include "custom/RGBLed.h"
#include <Arduino.h>
#include <DHT.h>

class ComponentManager {
public:
  static RGBLed led;
  /*
  static DHT tempHumSensor;


  // FIX: issue with this one is that appearantly needs 5V but our arduino only
  // has 3v (3v3) pin, either that or it this lib doesnt work.
  /*
  static NewPing distanceSensor;
  */

  static Accelerometer Accelerometer;

  static void initializeComponents();
};
