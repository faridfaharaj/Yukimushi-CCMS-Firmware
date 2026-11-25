#pragma once
#include "Reading.h"

class GeospatialReading : public Reading {
private:
  double latitude;
  double longitude;

public:
  explicit GeospatialReading(unsigned long stamp, double latitude,
                             double longitude)
      : Reading(stamp), latitude(latitude), longitude(longitude) {}

  explicit GeospatialReading() : Reading(0), latitude(0.0), longitude(0.0) {}

  double getLatitude() const { return latitude; }
  double getLongitude() const { return longitude; }

  size_t serialize(uint8_t *buffer) const {
    size_t offset = Reading::serialize(buffer);

    memcpy(buffer + offset, &latitude, sizeof(latitude));
    offset += sizeof(latitude);

    memcpy(buffer + offset, &longitude, sizeof(longitude));
    offset += sizeof(longitude);

    return offset;
  }

  static GeospatialReading deserialize(const uint8_t *buffer) {
    unsigned long stamp;
    memcpy(&stamp, buffer, sizeof(stamp));

    double lat;
    memcpy(&lat, buffer + Reading::size(), sizeof(lat));

    double lng;
    memcpy(&lng, buffer + Reading::size() + sizeof(lat), sizeof(lng));

    return GeospatialReading(stamp, lat, lng);
  }

  static constexpr size_t size() {
    return Reading::size() + 2 * sizeof(double);
  }

  static constexpr const char *getName() { return "Geospatial"; }
};
