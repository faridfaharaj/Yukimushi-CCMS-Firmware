// INFO: only works on analog (A0) pin, use a multiplexer
#pragma once
#include "Component.h"
#include "pins/InputPinBase.h"

class LDR : public Component<1> {

private:
  static constexpr float multValue = 32017200;
  static constexpr float powValue = 1.5832;

  unsigned long externalResistance;
  bool photocellOnGround;
  unsigned int adcResolutionBits;

  float rawAnalogValueToLux(int raw) const {
    unsigned long photocellResistor;
    int rawV = raw;

    if (pow(2, adcResolutionBits) == rawV) {
      rawV--;
    }

    float ratio = ((float)pow(2, adcResolutionBits) / (float)rawV) - 1;
    if (photocellOnGround) {
      photocellResistor = externalResistance / ratio;
    } else {
      photocellResistor = externalResistance * ratio;
    }

    return multValue / (float)pow(photocellResistor, powValue);
  }

  float luxToFootCandles(float lux) { return lux / 10.764; }

public:
  explicit LDR(InputPinBase &pin, unsigned long externalResistance,
               bool photocellOnGround = false,
               unsigned int adcResolutionBits = 10)
      : Component<1>({&pin}), externalResistance(externalResistance),
        adcResolutionBits(adcResolutionBits) {}

  float readLux() { return rawAnalogValueToLux(getPin(0)->readAnalog()); }
  float readFootCandles() { return luxToFootCandles(readLux()); }
};
