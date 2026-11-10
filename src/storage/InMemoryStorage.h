#pragma once

namespace InMemoryStorage {

void init();

void storeCover(bool);
void storeHumidity(float);
void storeLight(float);
void storeShock();
void storeTemperature(float);
void storeTipped(float);

} // namespace InMemoryStorage
