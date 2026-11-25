#pragma once
#include "Component.h"
#include "pins/InputPinBase.h"
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

class GPS : public Component<2> {
private:
  TinyGPSPlus gps;
  SoftwareSerial gpsSerial;

public:
  explicit GPS(InputPinBase &rx, InputPinBase &tx)
      : Component<2>({&rx, &tx}), gpsSerial(rx.getPin(), tx.getPin()) {}

  bool getCoords(double &outLat, double &outLng) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }

    if (gps.location.isValid()) {
      outLat = gps.location.lat();
      outLng = gps.location.lng();
      return true;
    }
    return false;
  }

  void init(uint32_t baud) { gpsSerial.begin(baud); }
};
