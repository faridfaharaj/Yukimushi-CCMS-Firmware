#pragma once

namespace Messaging {

void init(const char *topicCover, const char *topicHumidity,
          const char *topicLight, const char *topicShock,
          const char *topicTemperature, const char *topicTipped);

void publishStorage();

} // namespace Messaging
