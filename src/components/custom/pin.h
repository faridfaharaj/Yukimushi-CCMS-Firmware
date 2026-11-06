#pragma once
#include <Arduino.h>
#include <cstdint>

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

class InputPinBase : public Pin {};
class OutputPinBase : public Pin {};

class inPin : public InputPinBase {
  uint8_t pin;

public:
  explicit inPin(uint8_t pin) : pin(pin) {}
  void begin() override { pinMode(pin, INPUT); }
  int readAnalog() override { return analogRead(pin); }
  int readDigital() override { return digitalRead(pin); }
  uint8_t getPin() override { return pin; }
};

class inPullupPin : public InputPinBase {
  uint8_t pin;

public:
  explicit inPullupPin(uint8_t pin) : pin(pin) {}
  void begin() override { pinMode(pin, INPUT_PULLUP); }
  int readAnalog() override { return analogRead(pin); }
  int readDigital() override { return digitalRead(pin); }
  uint8_t getPin() override { return pin; }
};

class outPin : public OutputPinBase {
  uint8_t pin;

public:
  explicit outPin(uint8_t pin) : pin(pin) {}
  void begin() override { pinMode(pin, OUTPUT); }
  void writeAnalog(int val) override { analogWrite(pin, val); }
  void writeDigital(int val) override { digitalWrite(pin, val); }
  uint8_t getPin() override { return pin; }
};

struct ChannelSource {
  virtual int readAnalog(uint8_t ch) = 0;
  virtual int readDigital(uint8_t ch) = 0;
  virtual void writeAnalog(uint8_t ch, int val) = 0;
  virtual void writeDigital(uint8_t ch, int val) = 0;
  virtual uint8_t getZPin(uint8_t ch) = 0;
};

class PlexedInPin : public InputPinBase {
  ChannelSource &src;
  uint8_t ch;

public:
  PlexedInPin(ChannelSource &src, uint8_t ch) : src(src), ch(ch) {}
  int readAnalog() override { return src.readAnalog(ch); }
  int readDigital() override { return src.readDigital(ch); }
};

class PlexedOutPin : public OutputPinBase {
  ChannelSource &src;
  uint8_t ch;

public:
  PlexedOutPin(ChannelSource &src, uint8_t ch) : src(src), ch(ch) {}
  void writeAnalog(int val) override { src.writeAnalog(ch, val); }
  void writeDigital(int val) override { src.writeDigital(ch, val); }
  uint8_t getPin() override { return src.getZPin(ch); }
};
