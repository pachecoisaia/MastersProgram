#include "differential_drive.h"
#include "../utils/logger.h"
#include "../utils/util.h"

// Default wheelbase for Pololu 3pi+ robot (distance between left and right wheels)
static const float DEFAULT_WHEELBASE_MM = 96.0f;

DifferentialDrive::DifferentialDrive() {
  Logger::log_info(__FUNCTION__, "Differential drive initialized");
  motors = new Motors();
  turn_speed_ratio = 0.5f;
  wheelbase_mm = DEFAULT_WHEELBASE_MM;
}

void DifferentialDrive::set_wheel_speeds(int left_speed_mm_per_s, int right_speed_mm_per_s) {
  Logger::log_debug(__FUNCTION__, "Setting wheel speeds");
  motors->setSpeeds(left_speed_mm_per_s, right_speed_mm_per_s);
}

void DifferentialDrive::drive_forward(int speed_mm_per_s) {
  Logger::log_debug(__FUNCTION__, "Driving forward");
  motors->setSpeeds(speed_mm_per_s, speed_mm_per_s);
}

void DifferentialDrive::drive_backward(int speed_mm_per_s) {
  Logger::log_debug(__FUNCTION__, "Driving backward");
  motors->setSpeeds(-speed_mm_per_s, -speed_mm_per_s);
}

void DifferentialDrive::turn_left_low_level(int speed_mm_per_s) {
  Logger::log_debug(__FUNCTION__, "Turning left");
  motors->setSpeeds(-speed_mm_per_s, speed_mm_per_s);
}

void DifferentialDrive::turn_right_low_level(int speed_mm_per_s) {
  Logger::log_debug(__FUNCTION__, "Turning right");
  motors->setSpeeds(speed_mm_per_s, -speed_mm_per_s);
}

void DifferentialDrive::halt() {
  Logger::log_info(__FUNCTION__, "Differential drive halted");
  motors->setSpeeds(0, 0);
}

void DifferentialDrive::set_turn_speed_ratio(float ratio) {
  Logger::log_info(__FUNCTION__, "Setting turn speed ratio");
  try {
    validate_float(ratio, 0.0f, 1.0f);
    turn_speed_ratio = ratio;
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
  }
}

float DifferentialDrive::get_turn_speed_ratio() {
  return turn_speed_ratio;
}

void DifferentialDrive::set_wheelbase(float wheelbase_mm) {
  Logger::log_info(__FUNCTION__, "Setting wheelbase to " + std::to_string(wheelbase_mm) + " mm");
  try {
    validate_float(wheelbase_mm, 0.0f, 1000.0f);  // Max 1000mm wheelbase
    this->wheelbase_mm = wheelbase_mm;
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
  }
}

float DifferentialDrive::get_wheelbase() {
  return wheelbase_mm;
}

void DifferentialDrive::flip_left_motor(bool flip) {
  Logger::log_info(__FUNCTION__, "Flipping left motor");
  motors->flipLeftMotor(flip);
}

void DifferentialDrive::flip_right_motor(bool flip) {
  Logger::log_info(__FUNCTION__, "Flipping right motor");
  motors->flipRightMotor(flip);
}

// ========== HIGH-LEVEL MOTION PRIMITIVES ==========

void DifferentialDrive::move_forward(float distance_m, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "distance=" + std::to_string(distance_m) + " m, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(distance_m, 0.0f, 100.0f);   // Max 100m distance
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int speed = (int)(speed_m_per_s * 1000);
    int duration_ms = (int)((distance_m / speed_m_per_s) * 1000);
    
    drive_forward(speed);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::move_backward(float distance_m, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "distance=" + std::to_string(distance_m) + " m, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(distance_m, 0.0f, 100.0f);   // Max 100m distance
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int speed = (int)(speed_m_per_s * 1000);
    int duration_ms = (int)((distance_m / speed_m_per_s) * 1000);
    
    drive_backward(speed);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::turn_left(float angle_rad, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "angle=" + std::to_string(angle_rad) + " rad, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(angle_rad, 0.0f, 6.28319f);  // Max 2*PI radians (full rotation)
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int speed_mm_per_s = (int)(speed_m_per_s * 1000);
    // Using formula: theta = (2*v/L) * t
    // Solving for t: t = theta * L / (2*v)
    float duration_s = (angle_rad * wheelbase_mm) / (2.0f * speed_mm_per_s);
    int duration_ms = (int)(duration_s * 1000);
    
    turn_left_low_level(speed_mm_per_s);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::turn_left(float duration_s, float speed_m_per_s, TurnMode mode) {
  Logger::log_debug(__FUNCTION__, "duration=" + std::to_string(duration_s) + " s, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(duration_s, 0.0f, 60.0f);    // Max 60 seconds
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int speed_mm_per_s = (int)(speed_m_per_s * 1000);
    int duration_ms = (int)(duration_s * 1000);
    
    turn_left_low_level(speed_mm_per_s);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::turn_right(float angle_rad, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "angle=" + std::to_string(angle_rad) + " rad, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(angle_rad, 0.0f, 6.28319f);  // Max 2*PI radians (full rotation)
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int speed_mm_per_s = (int)(speed_m_per_s * 1000);
    // Using formula: theta = (2*v/L) * t
    // Solving for t: t = theta * L / (2*v)
    float duration_s = (angle_rad * wheelbase_mm) / (2.0f * speed_mm_per_s);
    int duration_ms = (int)(duration_s * 1000);
    
    turn_right_low_level(speed_mm_per_s);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::turn_right(float duration_s, float speed_m_per_s, TurnMode mode) {
  Logger::log_debug(__FUNCTION__, "duration=" + std::to_string(duration_s) + " s, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(duration_s, 0.0f, 60.0f);    // Max 60 seconds
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int speed_mm_per_s = (int)(speed_m_per_s * 1000);
    int duration_ms = (int)(duration_s * 1000);
    
    turn_right_low_level(speed_mm_per_s);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::move_forward_turning_left(float distance_m, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "distance=" + std::to_string(distance_m) + " m, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(distance_m, 0.0f, 100.0f);   // Max 100m distance
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int outer_speed = (int)(speed_m_per_s * 1000);
    int inner_speed = calculate_inner_speed(outer_speed, turn_speed_ratio);
    int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
    
    set_wheel_speeds(inner_speed, outer_speed);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::move_forward_turning_right(float distance_m, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "distance=" + std::to_string(distance_m) + " m, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(distance_m, 0.0f, 100.0f);   // Max 100m distance
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int outer_speed = (int)(speed_m_per_s * 1000);
    int inner_speed = calculate_inner_speed(outer_speed, turn_speed_ratio);
    int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
    
    set_wheel_speeds(outer_speed, inner_speed);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::move_backward_turning_left(float distance_m, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "distance=" + std::to_string(distance_m) + " m, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(distance_m, 0.0f, 100.0f);   // Max 100m distance
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int outer_speed = (int)(speed_m_per_s * 1000);
    int inner_speed = calculate_inner_speed(outer_speed, turn_speed_ratio);
    int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
    
    set_wheel_speeds(-inner_speed, -outer_speed);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}

void DifferentialDrive::move_backward_turning_right(float distance_m, float speed_m_per_s) {
  Logger::log_debug(__FUNCTION__, "distance=" + std::to_string(distance_m) + " m, speed=" + std::to_string(speed_m_per_s) + " m/s");
  
  try {
    validate_float(distance_m, 0.0f, 100.0f);   // Max 100m distance
    validate_float(speed_m_per_s, 0.0f, 0.4f);  // Max 0.4 m/s speed
    
    int outer_speed = (int)(speed_m_per_s * 1000);
    int inner_speed = calculate_inner_speed(outer_speed, turn_speed_ratio);
    int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
    
    set_wheel_speeds(-outer_speed, -inner_speed);
    
    delay(duration_ms);
    halt();
  } catch (const std::out_of_range& e) {
    Logger::log_error(__FUNCTION__, e.what());
    halt();
  }
}
