#pragma once

namespace Messaging {

void init(const char *topicCover, const char *topicHumidity,
          const char *topicLight, const char *topicShock,
          const char *topicTemperature, const char *topicTipped,
          const char *topicGeospatial);

void publishStorage();

} // namespace Messaging
