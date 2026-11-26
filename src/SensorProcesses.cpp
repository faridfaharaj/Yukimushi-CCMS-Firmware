#include "SensorProcesses.h"
#include "components/ComponentRegistry.h"
#include "util.h"
#include <cmath>

namespace SensorProcesses {

constexpr unsigned long LIGHT_RATE_MS = 60000;
constexpr unsigned long ACCELERATION_RATE_MS = 20;
constexpr unsigned long SHOCK_COOLDOWN_MS = 1000;
constexpr float SHOCK_THRESHOLD_G = 2.0f;
constexpr float TIPPED_DEGREES = 45.0f;
constexpr int TILT_SAMPLES = 50;

constexpr unsigned long TEMPERATURE_RATE_MS = 60000;

constexpr unsigned long HUMIDITY_RATE_MS = 60000;

constexpr unsigned long COVER_RATE_MS = 1000;
constexpr unsigned long COVER_DISTANCE_CM = 3;

constexpr unsigned long GEOSPATIAL_RATE_MS = 500;

bool humidityNTemperatureEnabled;
bool accelerationEnabled;
bool lightEnabled;
bool proximityEnabled;
bool geospatialEnabled;
bool bluetoothEnabled;

Event<4, GeospatialReading &> onReadGPS;
Event<4, LightReading &> onReadLight;
Event<4, HumidityReading &> onReadHumidity;
Event<4, TemperatureReading &> onReadTemperature;

Event<4, TippedReading &> onTipped;
// NOTE: shock GY-61 can only read up to 3g so shock magnitude cannot really be
// measured unless we have a better sensor
Event<4, ShockReading &> onShock;
Event<4, CoverReading &> onCoverChanged;

void readLight() {
  if (!lightEnabled)
    return;

  static unsigned long last = 0;
  if (!due(last, LIGHT_RATE_MS))
    return;

  float lux = ComponentRegistry::ldr.readLux();

  LightReading lightR(time(nullptr), lux);
  onReadLight.emit(lightR);
}

void readAcceleration() {
  if (!accelerationEnabled)
    return;

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

    ShockReading shockR(time(nullptr));
    onShock.emit(shockR);
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
      TippedReading tippedR(time(nullptr));
      onTipped.emit(tippedR);
    }
  }
}

void readTemperature() {
  if (!humidityNTemperatureEnabled)
    return;

  static unsigned long last = 0;
  if (!due(last, TEMPERATURE_RATE_MS))
    return;

  float degrees = ComponentRegistry::tempHumSensor.readTemperature();

  TemperatureReading temperatureR(time(nullptr), degrees);
  onReadTemperature.emit(temperatureR);
}

void readHumidity() {
  if (!humidityNTemperatureEnabled)
    return;

  static unsigned long last = 0;
  if (!due(last, HUMIDITY_RATE_MS))
    return;

  float humidity = ComponentRegistry::tempHumSensor.readHumidity();

  HumidityReading humidityR(time(nullptr), humidity);
  onReadHumidity.emit(humidityR);
}

void readCovered() {
  if (!proximityEnabled)
    return;

  static unsigned long last = 0;
  if (!due(last, COVER_RATE_MS))
    return;

  unsigned int cm = ComponentRegistry::distanceSensor.ping_cm();
  bool hasCovered = cm > COVER_DISTANCE_CM;

  static bool wasCovered = hasCovered;

  if (wasCovered != hasCovered) {
    CoverReading coverR(time(nullptr), hasCovered);
    onCoverChanged.emit(coverR);
  }

  wasCovered = hasCovered;
}

void readGeospatial() {
  if (!geospatialEnabled)
    return;

  double longitude;
  double latitude;
  if (!ComponentRegistry::gps.getCoords(latitude, longitude))
    return;

  static unsigned long last = 0;
  if (!due(last, GEOSPATIAL_RATE_MS))
    return;

  GeospatialReading geostpatialR(time(nullptr), latitude, longitude);
  onReadGPS.emit(geostpatialR);
}

} // namespace SensorProcesses

void SensorProcesses::init(bool enableHumidityNTemperature,
                           bool enableAcceleration, bool enableLight,
                           bool enableProximity, bool enableGeospatial) {

  humidityNTemperatureEnabled = enableHumidityNTemperature;
  accelerationEnabled = enableAcceleration;
  lightEnabled = enableLight;
  proximityEnabled = enableProximity;
  geospatialEnabled = enableGeospatial;
}

void SensorProcesses::update() {
  SensorProcesses::readGeospatial();
  SensorProcesses::readLight();
  SensorProcesses::readAcceleration();
  SensorProcesses::readHumidity();
  SensorProcesses::readTemperature();
  SensorProcesses::readCovered();
}
