#pragma once
#include "Component.h"
#include "pins/UninitializedPin.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

class GPS : public Component<2> {
private:
  TinyGPSPlus gps;
  SoftwareSerial gpsSerial;

public:
  explicit GPS(UninitializedPin &rx, UninitializedPin &tx)
      : Component<2>({&rx, &tx}), gpsSerial(rx.getPin(), tx.getPin()) {}

  bool getCoords(double &outLat, double &outLng, uint32_t maxAgeMs = 2000) {
    while (gpsSerial.available() > 0) {
      gps.encode(static_cast<char>(gpsSerial.read()));
    }

    if (!gps.location.isValid() || gps.location.age() > maxAgeMs) {
      return false;
    }

    outLat = gps.location.lat();
    outLng = gps.location.lng();
    return true;
  }

  void init(uint32_t baud) { gpsSerial.begin(baud); }
};
