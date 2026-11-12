#pragma once
#include "containers/Queue.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <cstddef>

namespace DiskStorage {

// 4096 = 4KB
constexpr size_t MAXBYTESIZE = 24;

void read();
void RemoveFile();
void removeFirst();

template <size_t N, typename T> bool write(Queue<N, T> &queue) {
  if (queue.isEmpty())
    return true;

  char dirName[23];
  snprintf(dirName, sizeof(dirName), "/readings/%s/", T::getName());
  const size_t recSize = T::size();
  static unsigned long idx = 0;
  uint8_t buffer[T::size()];
  char path[44];

  while (!queue.isEmpty()) {
    snprintf(path, sizeof(path), "%sreading%lu.bin", dirName,
             (unsigned long)idx);

    File file = LittleFS.open(path, "a");
    if (!file) {
      Serial.printf("Couldn't open %s\n", path);
      return false;
    }

    size_t used = file.size();
    while (!queue.isEmpty() && used + recSize <= MAXBYTESIZE) {
      size_t n = queue.getFirst().serialize(buffer);
      if (n != recSize || file.write(buffer, n) != n) {
        file.close();
        return false;
      }
      queue.pop();
      used += recSize;
    }

    file.close();
    if (used + recSize > MAXBYTESIZE)
      idx++;
  }

  return true;
}

} // namespace DiskStorage
