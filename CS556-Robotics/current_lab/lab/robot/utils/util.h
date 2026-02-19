#ifndef util_h
#define util_h

#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Utility functions for unit conversions
int convert_speed_to_mm_per_s(float speed, float base_speed);
int convert_duration_to_ms(float duration);
int calculate_duration_ms(float distance, float speed, float base_speed);

// Utility function for calculating inner wheel speed for curved motion
int calculate_inner_speed(int outer_speed_mm_per_s, float turn_speed_ratio);

// Utility function for calculating motion duration in milliseconds
int calculate_motion_duration_ms(float distance_m, float speed_m_per_s);

// Utility function for boundary validation - returns false if out of bounds
bool validate_float(float value, float min, float max);

// Mathematical conversion utilities

// Angle conversions
float degrees_to_radians(float degrees);
float radians_to_degrees(float radians);

// Normalize angle to [-PI, PI] range
float normalize_angle_radians(float angle);

// Normalize angle to [-180, 180] range
float normalize_angle_degrees(float angle);

// Angular velocity conversions
float rpm_to_rad_per_s(float rpm);
float rad_per_s_to_rpm(float rad_per_s);

// Distance conversions
float meters_to_millimeters(float meters);
float millimeters_to_meters(float millimeters);
float inches_to_millimeters(float inches);
float millimeters_to_inches(float millimeters);

// Speed conversions
float meters_per_s_to_mm_per_s(float m_per_s);
float mm_per_s_to_meters_per_s(float mm_per_s);

// Differential drive turn duration calculation
// Calculates duration (seconds) needed to turn angle_rad at given speed
// Using formula: theta = (2*v/L) * t => t = theta * L / (2*v)
float calculate_turn_duration_s(float angle_rad, float wheelbase_mm, float speed_mm_per_s);

// ========== Odometry math helpers ==========

// Distance traveled by a wheel for a given encoder count (increment).
float compute_wheel_distance(int counts, float diameter, float counts_per_rev);

// Convert encoder counts-per-motor-rev (n) to counts-per-wheel-rev.
float compute_ticks_per_rev(float n, float gearRatio);

// Per-wheel arc lengths from encoder count deltas.
float compute_delta_l(int32_t leftCountsDelta, float wheelDiameterL, float leftCountsPerWheelRev);
float compute_delta_r(int32_t rightCountsDelta, float wheelDiameterR, float rightCountsPerWheelRev);

// Heading change from wheel deltas.
float compute_d_theta(float delta_r, float delta_l, float w);

// Heading change from gyro z-rate (deg/s) over dt (s).
float compute_dtheta_imu(float gyro_z, float dt);

// Body-frame translation projected into world frame.
float compute_delta_x(float delta_l, float delta_r, float theta);
float compute_delta_y(float delta_l, float delta_r, float theta);

#endif
