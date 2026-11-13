#include "InMemoryStorage.h"
#include "DiskStorage.h"

namespace InMemoryStorage {

Queue<416, HumidityReading> humidity;
Queue<416, TemperatureReading> temperature;
Queue<416, LightReading> light;

Queue<666, CoverReading> cover;

Queue<833, ShockReading> shock;
Queue<833, TippedReading> tipped;

template <size_t N, typename T> void push(Queue<N, T> &q, const T &reading) {
  if (q.isFull() && !DiskStorage::write<N, T>(q))
    q.pop();
  q.push(reading);
}

void store(const HumidityReading &r) { push(humidity, r); }
void store(const TemperatureReading &r) { push(temperature, r); }
void store(const LightReading &r) { push(light, r); }
void store(const CoverReading &r) { push(cover, r); }
void store(const ShockReading &r) { push(shock, r); }
void store(const TippedReading &r) { push(tipped, r); }

template <size_t N, typename T> bool retrieve(Queue<N, T> &q, T &outR) {
  if (q.isEmpty())
    return false;

  outR = q.getFirst();
  q.pop();

  return true;
}

template <size_t N, typename T> bool diskFill(Queue<N, T> &q) {
  if (!q.isEmpty())
    return false;

  return DiskStorage::load(q);
  return true;
}

bool load(HumidityReading &r) { return retrieve(humidity, r); }
bool load(TemperatureReading &r) { return retrieve(temperature, r); }
bool load(LightReading &r) { return retrieve(light, r); }
bool load(CoverReading &r) { return retrieve(cover, r); }
bool load(ShockReading &r) { return retrieve(shock, r); }
bool load(TippedReading &r) { return retrieve(tipped, r); }

bool fill(const HumidityReading &r) { return diskFill(humidity); }
bool fill(const TemperatureReading &r) { return diskFill(temperature); }
bool fill(const LightReading &r) { return diskFill(light); }
bool fill(const CoverReading &r) { return diskFill(cover); }
bool fill(const ShockReading &r) { return diskFill(shock); }
bool fill(const TippedReading &r) { return diskFill(tipped); }

} // namespace InMemoryStorage
