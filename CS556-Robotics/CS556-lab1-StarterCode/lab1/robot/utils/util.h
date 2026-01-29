#ifndef util_h
#define util_h
#include <stdexcept>

// Utility functions for unit conversions
int convert_speed_to_mm_per_s(float speed, float base_speed);
int convert_duration_to_ms(float duration);
int calculate_duration_ms(float distance, float speed, float base_speed);

// Utility function for calculating inner wheel speed for curved motion
int calculate_inner_speed(int outer_speed_mm_per_s, float turn_speed_ratio);

// Utility function for calculating motion duration in milliseconds
int calculate_motion_duration_ms(float distance_m, float speed_m_per_s);

// Utility function for boundary validation - throws std::out_of_range if out of bounds
float validate_float(float value, float min, float max);

#endif
