#include "util.h"

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

// Normalize angle in radians to [-PI, PI] range
float normalize_angle_radians(float angle) {
  while (angle > M_PI) angle -= 2.0 * M_PI;
  while (angle < -M_PI) angle += 2.0 * M_PI;
  return angle;
}

// Normalize angle in degrees to [-180, 180] range
float normalize_angle_degrees(float angle) {
  while (angle > 180.0) angle -= 360.0;
  while (angle < -180.0) angle += 360.0;
  return angle;
}

// Convert RPM (revolutions per minute) to radians per second
float rpm_to_rad_per_s(float rpm) {
  return rpm * 2.0 * M_PI / 60.0;
}

// Convert radians per second to RPM
float rad_per_s_to_rpm(float rad_per_s) {
  return rad_per_s * 60.0 / (2.0 * M_PI);
}

// Convert meters to millimeters
float meters_to_millimeters(float meters) {
  return meters * 1000.0;
}

// Convert millimeters to meters
float millimeters_to_meters(float millimeters) {
  return millimeters / 1000.0;
}

// Convert inches to millimeters
float inches_to_millimeters(float inches) {
  return inches * 25.4;
}

// Convert millimeters to inches
float millimeters_to_inches(float millimeters) {
  return millimeters / 25.4;
}

// Convert meters per second to millimeters per second
float meters_per_s_to_mm_per_s(float m_per_s) {
  return m_per_s * 1000.0;
}

// Convert millimeters per second to meters per second
float mm_per_s_to_meters_per_s(float mm_per_s) {
  return mm_per_s / 1000.0;
}

// Calculate turn duration for differential drive
// Using formula: theta = (2*v/L) * t
// Solving for t: t = theta * L / (2*v)
float calculate_turn_duration_s(float angle_rad, float wheelbase_mm, float speed_mm_per_s) {
  return (angle_rad * wheelbase_mm) / (2.0f * speed_mm_per_s);
}

// ========== Odometry math helpers ==========

float compute_wheel_distance(int counts, float diameter, float counts_per_rev) {
  return (float)(M_PI * diameter * (float)counts / counts_per_rev);
}

float compute_ticks_per_rev(float n, float gearRatio) {
  return n * gearRatio;
}

float compute_delta_l(float leftCountsDelta, float wheelDiameterL, float leftCountsPerWheelRev) {
  return (float)(M_PI * wheelDiameterL * (float)leftCountsDelta / leftCountsPerWheelRev);
}

float compute_delta_r(float rightCountsDelta, float wheelDiameterR, float rightCountsPerWheelRev) {
  return (float)(M_PI * wheelDiameterR * (float)rightCountsDelta / rightCountsPerWheelRev);
}

float compute_d_theta(float delta_r, float delta_l, float w) {
  return (delta_r - delta_l) / w;
}

float compute_dtheta_imu(float gyro_z, float dt) {
  return gyro_z * 0.0174533f * dt; // deg to rad
}

float compute_delta_x(float delta_l, float delta_r, float theta) {
  return (delta_l + delta_r) / 2.0f * cosf(theta);
}

float compute_delta_y(float delta_l, float delta_r, float theta) {
  return (delta_l + delta_r) / 2.0f * sinf(theta);
}
