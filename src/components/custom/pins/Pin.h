#pragma once

class Pin {
public:
  virtual ~Pin() {}
  virtual void begin() {}
  virtual int readAnalog() { return 0; }
  virtual int readDigital() { return 0; }
  virtual void writeAnalog(int) {}
  virtual void writeDigital(int) {}
  virtual uint8_t getPin() { return 0; }
};

struct ChannelSource {
  virtual int readAnalog(uint8_t ch) = 0;
  virtual int readDigital(uint8_t ch) = 0;
  virtual void writeAnalog(uint8_t ch, int val) = 0;
  virtual void writeDigital(uint8_t ch, int val) = 0;
  virtual uint8_t getZPin(uint8_t ch) = 0;
};
