#include "util.h"

// Utility functions
int convert_duration_to_ms(float duration) {
  return (int)(duration * 1000);
}

int calculate_inner_speed(int outer_speed_mm_per_s, float turn_speed_ratio) {
  return (int)(outer_speed_mm_per_s * turn_speed_ratio);
}

int calculate_motion_duration_ms(float distance_m, float speed_m_per_s) {
  return (int)((distance_m / speed_m_per_s) * 1000);
}

// Validate a float value is within bounds, return false if out of range
bool validate_float(float value, float min, float max) {
  return (value >= min && value <= max);
}

// ========== Mathematical Conversion Utilities ==========

// Convert degrees to radians
float degrees_to_radians(float degrees) {
  return degrees * M_PI / 180.0;
}

// Convert radians to degrees
float radians_to_degrees(float radians) {
  return radians * 180.0 / M_PI;
}
// Convert meters per second to millimeters per second
float meters_per_s_to_mm_per_s(float m_per_s) {
  return m_per_s * 1000.0;
}

// Calculate turn duration for differential drive
// Using formula: theta = (2*v/L) * t
// Solving for t: t = theta * L / (2*v)
float calculate_turn_duration_s(float angle_rad, float wheelbase_mm, float speed_mm_per_s) {
  return (angle_rad * wheelbase_mm) / (2.0f * speed_mm_per_s);
}

// ========== Odometry math helpers ==========

float compute_delta_l(int32_t leftCountsDelta, float wheelDiameterL, float leftCountsPerWheelRev) {
  return (float)(M_PI * wheelDiameterL * (float)leftCountsDelta / leftCountsPerWheelRev);
}

float compute_delta_r(int32_t rightCountsDelta, float wheelDiameterR, float rightCountsPerWheelRev) {
  return (float)(M_PI * wheelDiameterR * (float)rightCountsDelta / rightCountsPerWheelRev);
}

float compute_delta_x(float delta_l, float delta_r, float theta) {
  return (delta_l + delta_r) / 2.0f * cosf(theta);
}

float compute_delta_y(float delta_l, float delta_r, float theta) {
  return (delta_l + delta_r) / 2.0f * sinf(theta);
}
