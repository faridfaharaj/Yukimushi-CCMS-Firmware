#pragma once
#include <stddef.h>

template <size_t N, typename T> class Deque {
private:
  T entries[N];

  size_t firstIdx = 0;
  size_t count = 0;

  size_t lastIdx() const { return (firstIdx + count - 1) % N; }

public:
  bool isEmpty() const { return count == 0; }

  bool isFull() const { return count == N; }

  size_t size() const { return count; }

  T &getFirst() { return entries[firstIdx]; }

  T &getLast() { return entries[lastIdx()]; }

  bool pushFirst(const T &obj) {
    if (isFull())
      return false;

    firstIdx = (firstIdx + N - 1) % N;
    entries[firstIdx] = obj;
    count++;

    return true;
  }

  bool pushLast(const T &obj) {
    if (isFull())
      return false;

    size_t idx = (firstIdx + count) % N;
    entries[idx] = obj;
    count++;

    return true;
  }

  bool popFirst() {
    if (isEmpty())
      return false;

    firstIdx = (firstIdx + 1) % N;
    count--;

    return true;
  }

  bool popLast() {
    if (isEmpty())
      return false;

    count--;

    return true;
  }
};
