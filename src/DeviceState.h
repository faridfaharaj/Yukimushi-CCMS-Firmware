#pragma once
#include <cstdint>

enum class DeviceState : uint8_t {
  CONFIG_PENDING = 0,
  CONFIG_ERROR = 1,
  OFFLINE = 2,
  ONLINE = 3,
};

enum class MqttTopicMessageType : uint8_t {
    DBIRTH,
    DDEATH,
    DDATA,
    NDATA,
    NCMD,
    DCMD,
    STATEU
};

constexpr const char* toString(MqttTopicMessageType type) {
    switch (type) {
        case MqttTopicMessageType::DBIRTH: return "DBIRTH";
        case MqttTopicMessageType::DDEATH: return "DDEATH";
        case MqttTopicMessageType::DDATA:  return "DDATA";
        case MqttTopicMessageType::NDATA:  return "NDATA";
        case MqttTopicMessageType::NCMD:   return "NCMD";
        case MqttTopicMessageType::DCMD:   return "DCMD";
        case MqttTopicMessageType::STATEU: return "STATU";
    }

    return "UNKNOWN";
}
