#include "util.h"
#include <string>

// Utility functions
int convert_speed_to_mm_per_s(float speed, float base_speed) {
  if (speed < 0) speed = base_speed;
  return (int)(speed * 1000);
}

int convert_duration_to_ms(float duration) {
  return (int)(duration * 1000);
}

int calculate_duration_ms(float distance, float speed, float base_speed) {
  if (speed < 0) speed = base_speed;
  return (int)((distance / speed) * 1000);
}

int calculate_inner_speed(int outer_speed_mm_per_s, float turn_speed_ratio) {
  return (int)(outer_speed_mm_per_s * turn_speed_ratio);
}

int calculate_motion_duration_ms(float distance_m, float speed_m_per_s) {
  return (int)((distance_m / speed_m_per_s) * 1000);
}

// Validate a float value is within bounds, throw if out of range
float validate_float(float value, float min, float max) {
  if (value < min || value > max) {
    throw std::out_of_range("Value " + std::to_string(value) + 
                            " is out of bounds [" + std::to_string(min) + 
                            ", " + std::to_string(max) + "]");
  }
  return value;
}
