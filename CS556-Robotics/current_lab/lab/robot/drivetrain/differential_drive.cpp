#include "differential_drive.h"
#include "../utils/logger.h"
#include "../utils/util.h"

#undef CLASS_NAME
#define CLASS_NAME "DifferentialDrive"

DifferentialDrive::DifferentialDrive() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Initialized");
  turn_speed_ratio = DEFAULT_TURN_SPEED_RATIO;
  wheelbase_mm = DEFAULT_WHEELBASE_MM;
}

// ========== CONFIGURATION ==========

void DifferentialDrive::configure() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuring differential drive");
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Turn speed ratio: " + String(DEFAULT_TURN_SPEED_RATIO)).c_str());
  set_turn_speed_ratio(DEFAULT_TURN_SPEED_RATIO);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Wheelbase: " + String(DEFAULT_WHEELBASE_MM) + " mm").c_str());
  set_wheelbase(DEFAULT_WHEELBASE_MM);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Left motor flip: " + String(DEFAULT_FLIP_LEFT_MOTOR ? "true" : "false")).c_str());
  flip_left_motor(DEFAULT_FLIP_LEFT_MOTOR);
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Right motor flip: " + String(DEFAULT_FLIP_RIGHT_MOTOR ? "true" : "false")).c_str());
  flip_right_motor(DEFAULT_FLIP_RIGHT_MOTOR);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuration complete");
}

void DifferentialDrive::set_wheel_speeds(int left_speed_mm_per_s, int right_speed_mm_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Setting wheel speeds");
  motors.setSpeeds(left_speed_mm_per_s, right_speed_mm_per_s);
}

void DifferentialDrive::drive_forward(int speed_mm_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Driving forward");
  motors.setSpeeds(speed_mm_per_s, speed_mm_per_s);
}

void DifferentialDrive::drive_backward(int speed_mm_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Driving backward");
  motors.setSpeeds(-speed_mm_per_s, -speed_mm_per_s);
}

// ========== NON-BLOCKING CONTINUOUS HELPERS ==========

void DifferentialDrive::drive_forward_unbounded(int speed_mm_per_s) {
  drive_forward(speed_mm_per_s);
}

void DifferentialDrive::drive_backward_unbounded(int speed_mm_per_s) {
  drive_backward(speed_mm_per_s);
}

void DifferentialDrive::turn_left_unbounded(int speed_mm_per_s) {
  turn_left_low_level(speed_mm_per_s);
}

void DifferentialDrive::turn_right_unbounded(int speed_mm_per_s) {
  turn_right_low_level(speed_mm_per_s);
}

void DifferentialDrive::turn_left_low_level(int speed_mm_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Turning left");
  motors.setSpeeds(-speed_mm_per_s, speed_mm_per_s);
}

void DifferentialDrive::turn_right_low_level(int speed_mm_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Turning right");
  motors.setSpeeds(speed_mm_per_s, -speed_mm_per_s);
}

void DifferentialDrive::halt() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Halted");
  motors.setSpeeds(0, 0);
}

void DifferentialDrive::set_turn_speed_ratio(float ratio) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Setting turn speed ratio");
  if (validate_float(ratio, 0.0f, 1.0f)) {
    turn_speed_ratio = ratio;
  } else {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid turn speed ratio");
  }
}

float DifferentialDrive::get_turn_speed_ratio() {
  return turn_speed_ratio;
}

void DifferentialDrive::set_wheelbase(float wheelbase_mm) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Setting wheelbase to " + String(wheelbase_mm) + " mm").c_str());
  if (validate_float(wheelbase_mm, 0.0f, 1000.0f)) {
    this->wheelbase_mm = wheelbase_mm;
  } else {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid wheelbase value");
  }
}

float DifferentialDrive::get_wheelbase() {
  return wheelbase_mm;
}

void DifferentialDrive::flip_left_motor(bool flip) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Flipping left motor");
  motors.flipLeftMotor(flip);
}

void DifferentialDrive::flip_right_motor(bool flip) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Flipping right motor");
  motors.flipRightMotor(flip);
}

// ========== HIGH-LEVEL MOTION PRIMITIVES ==========

void DifferentialDrive::move_forward(float distance_m, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("distance=" + String(distance_m) + " m, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(distance_m, 0.0f, 100.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
  
  drive_forward(speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::move_backward(float distance_m, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("distance=" + String(distance_m) + " m, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(distance_m, 0.0f, 100.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
  
  drive_backward(speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::turn_left(float duration_s, float speed_m_per_s) {
  turn_left_duration(duration_s, speed_m_per_s);
}

void DifferentialDrive::turn_left(float thetaOrTime, float speed_m_per_s, TurnMode mode) {
  switch (mode) {
    case TurnMode::ANGLE:
      Logger::log_debug(CLASS_NAME, __FUNCTION__, ("ANGLE mode: angle=" + String(thetaOrTime) + " rad").c_str());
      turn_left_angle(thetaOrTime, speed_m_per_s);
      break;
    
    case TurnMode::DURATION:
      Logger::log_debug(CLASS_NAME, __FUNCTION__, ("DURATION mode: duration=" + String(thetaOrTime) + " s").c_str());
      turn_left_duration(thetaOrTime, speed_m_per_s);
      break;
  }
}

void DifferentialDrive::turn_right(float duration_s, float speed_m_per_s) {
  turn_right_duration(duration_s, speed_m_per_s);
}

void DifferentialDrive::turn_right(float thetaOrTime, float speed_m_per_s, TurnMode mode) {
  switch (mode) {
    case TurnMode::ANGLE:
      Logger::log_debug(CLASS_NAME, __FUNCTION__, ("ANGLE mode: angle=" + String(thetaOrTime) + " rad").c_str());
      turn_right_angle(thetaOrTime, speed_m_per_s);
      break;
    
    case TurnMode::DURATION:
      Logger::log_debug(CLASS_NAME, __FUNCTION__, ("DURATION mode: duration=" + String(thetaOrTime) + " s").c_str());
      turn_right_duration(thetaOrTime, speed_m_per_s);
      break;
  }
}

void DifferentialDrive::turn_left_angle(float angle_rad, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("angle=" + String(angle_rad) + " rad, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(angle_rad, 0.0f, 6.28319f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  float duration_s = calculate_turn_duration_s(angle_rad, wheelbase_mm, speed_mm_per_s);
  int duration_ms = convert_duration_to_ms(duration_s);
  
  turn_left_low_level(speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::turn_right_angle(float angle_rad, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("angle=" + String(angle_rad) + " rad, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(angle_rad, 0.0f, 6.28319f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  float duration_s = calculate_turn_duration_s(angle_rad, wheelbase_mm, speed_mm_per_s);
  int duration_ms = convert_duration_to_ms(duration_s);
  
  turn_right_low_level(speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::turn_right_duration(float duration_s, float speed_m_per_s) {
  if (!validate_float(duration_s, 0.0f, 60.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int duration_ms = convert_duration_to_ms(duration_s);
  
  turn_right_low_level(speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::turn_left_duration(float duration_s, float speed_m_per_s) {
  if (!validate_float(duration_s, 0.0f, 60.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int duration_ms = convert_duration_to_ms(duration_s);
  
  turn_left_low_level(speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::move_forward_turning_left(float distance_m, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("distance=" + String(distance_m) + " m, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(distance_m, 0.0f, 100.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int outer_speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int inner_speed_mm_per_s = calculate_inner_speed(outer_speed_mm_per_s, turn_speed_ratio);
  int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
  
  set_wheel_speeds(inner_speed_mm_per_s, outer_speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::move_forward_turning_right(float distance_m, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("distance=" + String(distance_m) + " m, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(distance_m, 0.0f, 100.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int outer_speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int inner_speed_mm_per_s = calculate_inner_speed(outer_speed_mm_per_s, turn_speed_ratio);
  int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
  
  set_wheel_speeds(outer_speed_mm_per_s, inner_speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::move_backward_turning_left(float distance_m, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("distance=" + String(distance_m) + " m, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(distance_m, 0.0f, 100.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int outer_speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int inner_speed_mm_per_s = calculate_inner_speed(outer_speed_mm_per_s, turn_speed_ratio);
  int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
  
  set_wheel_speeds(-inner_speed_mm_per_s, -outer_speed_mm_per_s);
  delay(duration_ms);
  halt();
}

void DifferentialDrive::move_backward_turning_right(float distance_m, float speed_m_per_s) {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("distance=" + String(distance_m) + " m, speed=" + String(speed_m_per_s) + " m/s").c_str());
  
  if (!validate_float(distance_m, 0.0f, 100.0f) || !validate_float(speed_m_per_s, 0.0f, 0.4f)) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid parameters");
    halt();
    return;
  }
  
  int outer_speed_mm_per_s = (int)meters_per_s_to_mm_per_s(speed_m_per_s);
  int inner_speed_mm_per_s = calculate_inner_speed(outer_speed_mm_per_s, turn_speed_ratio);
  int duration_ms = calculate_motion_duration_ms(distance_m, speed_m_per_s);
  
  set_wheel_speeds(-outer_speed_mm_per_s, -inner_speed_mm_per_s);
  delay(duration_ms);
  halt();
}
