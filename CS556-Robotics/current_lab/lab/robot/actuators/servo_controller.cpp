#include "servo_controller.h"
#include "../utils/logger.h"
#include "../utils/util.h"

#undef CLASS_NAME
#define CLASS_NAME "ServoController"

ServoController::ServoController() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Initialized");
  current_angle = DEFAULT_SERVO_ANGLE;
  pin = DEFAULT_SERVO_PIN;
  speed_degrees_per_sec = DEFAULT_SERVO_SPEED;
  attached = false;
}

// ========== CONFIGURATION ==========

void ServoController::configure() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuring servo controller");
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Pin: " + String(DEFAULT_SERVO_PIN)).c_str());
  attach(DEFAULT_SERVO_PIN);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Initial angle: " + String(DEFAULT_SERVO_ANGLE) + "°").c_str());
  set_angle(DEFAULT_SERVO_ANGLE);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Speed: " + String(DEFAULT_SERVO_SPEED) + "°/s").c_str());
  set_speed(DEFAULT_SERVO_SPEED);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuration complete");
}

void ServoController::attach(int pin) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Attaching servo to pin " + String(pin)).c_str());
  
  if (attached) {
    Logger::log_warning(CLASS_NAME, __FUNCTION__, "Servo already attached, detaching first");
    detach();
  }
  
  this->pin = pin;
  servo.attach(pin);
  attached = true;
  
  // Small delay to let servo stabilize
  delay(DEFAULT_SETTLING_TIME_MS);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Servo attached successfully");
}

void ServoController::detach() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Detaching servo");
  
  if (attached) {
    servo.detach();
    attached = false;
  } else {
    Logger::log_warning(CLASS_NAME, __FUNCTION__, "Servo not attached");
  }
}

bool ServoController::is_attached() {
  return attached;
}

void ServoController::set_speed(int degrees_per_second) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Setting speed to " + String(degrees_per_second) + "°/s").c_str());
  
  if (degrees_per_second >= 1 && degrees_per_second <= 180) {
    speed_degrees_per_sec = degrees_per_second;
  } else {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid speed (must be 1-180 °/s)");
  }
}

int ServoController::get_speed() {
  return speed_degrees_per_sec;
}

// ========== POSITION CONTROL ==========

void ServoController::set_angle(int angle) {
  if (!attached) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Servo not attached");
    return;
  }
  
  int constrained = constrain_angle(angle);
  
  if (constrained != angle) {
    Logger::log_warning(CLASS_NAME, __FUNCTION__, ("Angle " + String(angle) + "° out of range, constrained to " + String(constrained) + "°").c_str());
  }
  
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("Setting angle to " + String(constrained) + "°").c_str());
  servo.write(constrained);
  current_angle = constrained;
  
  delay(DEFAULT_SERVO_STEP_DELAY);
}

int ServoController::get_angle() {
  return current_angle;
}

void ServoController::move_to_angle(int target_angle) {
  if (!attached) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Servo not attached");
    return;
  }
  
  int target = constrain_angle(target_angle);
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Moving from " + String(current_angle) + "° to " + String(target) + "°").c_str());
  
  int step_delay = calculate_step_delay();
  int direction = (target > current_angle) ? 1 : -1;
  
  // Move one degree at a time for smooth motion
  while (current_angle != target) {
    current_angle += direction;
    servo.write(current_angle);
    delay(step_delay);
  }
  
  // Extra settling time at final position
  delay(DEFAULT_SETTLING_TIME_MS);
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Reached target angle " + String(target) + "°").c_str());
}

void ServoController::center() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Centering servo");
  move_to_angle(90);
}

void ServoController::move_to_min() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Moving to minimum position");
  move_to_angle(MIN_SERVO_ANGLE);
}

void ServoController::move_to_max() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Moving to maximum position");
  move_to_angle(MAX_SERVO_ANGLE);
}

// ========== SWEEP FUNCTIONS ==========

void ServoController::sweep(int min_angle, int max_angle, int num_sweeps) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Sweeping between " + String(min_angle) + "° and " + String(max_angle) + "° for " + String(num_sweeps) + " cycles").c_str());
  
  int constrained_min = constrain_angle(min_angle);
  int constrained_max = constrain_angle(max_angle);
  
  if (constrained_min > constrained_max) {
    // Swap if min > max
    int temp = constrained_min;
    constrained_min = constrained_max;
    constrained_max = temp;
  }
  
  for (int i = 0; i < num_sweeps; i++) {
    Logger::log_info(CLASS_NAME, __FUNCTION__, ("Sweep cycle " + String(i + 1) + "/" + String(num_sweeps)).c_str());
    move_to_angle(constrained_max);
    move_to_angle(constrained_min);
  }
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Sweep complete");
}

void ServoController::sweep_full_range(int num_sweeps) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Sweeping full range for " + String(num_sweeps) + " cycles").c_str());
  sweep(MIN_SERVO_ANGLE, MAX_SERVO_ANGLE, num_sweeps);
}

// ========== UTILITY FUNCTIONS ==========

bool ServoController::is_valid_angle(int angle) {
  return (angle >= MIN_SERVO_ANGLE && angle <= MAX_SERVO_ANGLE);
}

int ServoController::constrain_angle(int angle) {
  if (angle < MIN_SERVO_ANGLE) {
    return MIN_SERVO_ANGLE;
  } else if (angle > MAX_SERVO_ANGLE) {
    return MAX_SERVO_ANGLE;
  }
  return angle;
}

// ========== PRIVATE HELPER FUNCTIONS ==========

int ServoController::calculate_step_delay() {
  // Calculate delay between 1-degree steps based on speed setting
  // speed_degrees_per_sec degrees/second → delay = 1000 / speed_degrees_per_sec ms/degree
  int delay_ms = 1000 / speed_degrees_per_sec;
  
  // Ensure minimum delay for servo stability
  if (delay_ms < DEFAULT_SERVO_STEP_DELAY) {
    delay_ms = DEFAULT_SERVO_STEP_DELAY;
  }
  
  return delay_ms;
}
