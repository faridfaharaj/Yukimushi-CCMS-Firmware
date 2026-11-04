#include "ComponentManager.h"
#include <NewPing.h>

RGBLed ComponentManager::led(D0, D1, D2);
/*
DHT ComponentManager::tempHumSensor(D3, DHT11);
NewPing ComponentManager::distanceSensor(D5, D6, 400);
*/

Accelerometer ComponentManager::Accelerometer(A0, D3, D4);

void ComponentManager::initializeComponents() { /*tempHumSensor.begin();*/ };
