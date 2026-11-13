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

} // namespace InMemoryStorage
