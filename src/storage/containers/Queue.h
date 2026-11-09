#pragma once
#include <stddef.h>

template <size_t N, typename T> class Queue {
private:
  T entries[N];

  size_t head = 0;  // First element
  size_t count = 0; // Number of elements

public:
  bool isEmpty() const { return count == 0; }

  bool isFull() const { return count == N; }

  size_t size() const { return count; }

  bool push(const T &obj) {
    if (isFull())
      return false;

    size_t tail = (head + count) % N;

    entries[tail] = obj;
    count++;

    return true;
  }

  T &getFirst() { return entries[head]; }

  const T &getFirst() const { return entries[head]; }

  bool pop() {
    if (isEmpty())
      return false;

    head = (head + 1) % N;
    count--;

    return true;
  }
};
