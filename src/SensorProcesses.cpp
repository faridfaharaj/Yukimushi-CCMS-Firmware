#include "SensorProcesses.h"
#include "components/ComponentRegistry.h"
#include "util.h"
#include <cmath>

namespace SensorProcesses {
Event<4, double> onReadLight;
Event<4, float> onReadHumidity;
Event<4, float> onReadTemperature;

Event<4, float> onTipped;
// NOTE: shock GY-61 can only read up to 3g so shock magnitude cannot really be
// measured unless we have a better sensor
Event<4> onShockDetected;
Event<4, bool> onCoverChanged;

constexpr unsigned long LIGHT_RATE_MS = 60000;

constexpr unsigned long ACCELERATION_RATE_MS = 20;
constexpr unsigned long SHOCK_COOLDOWN_MS = 500;
constexpr float SHOCK_THRESHOLD_G = 2.0f;
constexpr float TIPPED_DEGREES = 45.0f;
constexpr int TILT_SAMPLES = 50;

constexpr unsigned long TEMPERATURE_RATE_MS = 600000;

constexpr unsigned long HUMIDITY_RATE_MS = 600000;

constexpr unsigned long COVER_RATE_MS = 1000;
constexpr unsigned long COVER_DISTANCE_CM = 3;

void readLight() {
  static unsigned long last = 0;
  if (!due(last, LIGHT_RATE_MS))
    return;

  double lux = ComponentRegistry::ldr.readLux();
  onReadLight.emit(lux);
}

void readAcceleration() {
  static unsigned long last = 0;
  if (!due(last, ACCELERATION_RATE_MS))
    return;

  float accX, accY, accZ;
  ComponentRegistry::accelerometer.readAcceleration(accX, accY, accZ);

  static unsigned long shockCooldown = 0;
  float mag2 = sqrMagnitude(accX, accY, accZ);

  // Shock
  if (mag2 >= SHOCK_THRESHOLD_G * SHOCK_THRESHOLD_G &&
      cooldownDue(shockCooldown, SHOCK_COOLDOWN_MS)) {
    shockCooldown = millis();
    onShockDetected.emit();
  }

  // Tilt
  static float sx = 0, sy = 0, sz = 0;
  static int n = 0;

  float m2 = accX * accX + accY * accY + accZ * accZ;
  if (m2 < 0.81f || m2 > 1.21f) {
    sx = sy = sz = 0;
    n = 0;
    return;
  }

  sx += accX;
  sy += accY;
  sz += accZ;
  if (++n < TILT_SAMPLES)
    return;
  float ax = sx / n, ay = sy / n, az = sz / n;
  sx = sy = sz = 0;
  n = 0;

  // float tilt = atan2f(sqrtf(ax * ax + ay * ay), az) * 180.0f / PI;

  static float baseX, baseY, baseZ;
  static bool haveBase = false;
  if (!haveBase) {
    baseX = ax;
    baseY = ay;
    baseZ = az;
    haveBase = true;
    return;
  }

  float dot = ax * baseX + ay * baseY + az * baseZ;
  float mc = sqrtf(ax * ax + ay * ay + az * az);
  float mb = sqrtf(baseX * baseX + baseY * baseY + baseZ * baseZ);
  float change = acosf(constrain(dot / (mc * mb), -1.0f, 1.0f)) * 180.0f / PI;

  static bool wasTipped = false;
  bool hasTipped = change > TIPPED_DEGREES;
  if (wasTipped != hasTipped) {
    wasTipped = hasTipped;
    if (hasTipped) {
      onTipped.emit(change);
    }
  }
}

void readTemperature() {
  static unsigned long last = 0;
  if (!due(last, TEMPERATURE_RATE_MS))
    return;

  float degrees = ComponentRegistry::tempHumSensor.readTemperature();
  onReadTemperature.emit(degrees);
}

void readHumidity() {
  static unsigned long last = 0;
  if (!due(last, HUMIDITY_RATE_MS))
    return;

  float humidity = ComponentRegistry::tempHumSensor.readHumidity();
  onReadHumidity.emit(humidity);
}

void readCovered() {
  static unsigned long last = 0;
  if (!due(last, COVER_RATE_MS))
    return;

  unsigned int cm = ComponentRegistry::distanceSensor.readDistanceCM();
  bool hasCovered = cm > COVER_DISTANCE_CM;
  static bool wasCovered = hasCovered;
  if (wasCovered != hasCovered) {
    onCoverChanged.emit(cm > COVER_DISTANCE_CM);
  }
  wasCovered = hasCovered;
}

} // namespace SensorProcesses

void SensorProcesses::readbtn(unsigned long time) {
  Serial.println("ButtonPressed! for: " + String(time) + "ms");
}

void SensorProcesses::init() {
  ComponentRegistry::btn.onRelease.subscribe(readbtn);
}

void SensorProcesses::update() {
  SensorProcesses::readLight();
  SensorProcesses::readAcceleration();
  SensorProcesses::readHumidity();
  SensorProcesses::readTemperature();
  SensorProcesses::readCovered();
}
