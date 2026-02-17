#include "servo_controller_tests.h"
#include "../robot.h"
#include "../utils/logger.h"
#include <Arduino.h>

#undef CLASS_NAME
#define CLASS_NAME "ServoControllerTests"

// External robot instance from lab1.ino or lab2.ino
extern Robot robot;

void test_servo_center() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Center servo");
  
  robot.servo->center();
  int angle = robot.servo->get_angle();
  
  String msg = "Servo centered at " + String(angle) + "°";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  
  delay(2000);
}

void test_servo_move_to_angles() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Move to specific angles");
  
  // Move to first test angle
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Moving to " + String(TEST_ANGLE_1) + "°").c_str());
  robot.servo->move_to_angle(TEST_ANGLE_1);
  delay(1000);
  
  // Move to second test angle
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Moving to " + String(TEST_ANGLE_2) + "°").c_str());
  robot.servo->move_to_angle(TEST_ANGLE_2);
  delay(1000);
  
  // Return to center
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Returning to center");
  robot.servo->center();
  delay(1000);
}

void test_servo_move_to_limits() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Move to limit positions");
  
  // Move to minimum
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Moving to minimum (0°)");
  robot.servo->move_to_min();
  delay(1000);
  
  // Move to maximum
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Moving to maximum (180°)");
  robot.servo->move_to_max();
  delay(1000);
  
  // Return to center
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Returning to center");
  robot.servo->center();
  delay(1000);
}

void test_servo_smooth_movement() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Smooth movement between angles");
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Smooth move to 0°");
  robot.servo->move_to_angle(0);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Smooth move to 90°");
  robot.servo->move_to_angle(90);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Smooth move to 180°");
  robot.servo->move_to_angle(180);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Smooth move back to 90°");
  robot.servo->move_to_angle(90);
  
  delay(1000);
}

void test_servo_sweep() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Servo sweep");
  
  // Sweep a limited range
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Sweeping 45° to 135° for " + String(TEST_SWEEP_CYCLES) + " cycles").c_str());
  robot.servo->sweep(45, 135, TEST_SWEEP_CYCLES);
  
  delay(1000);
  
  // Sweep full range
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Sweeping full range for " + String(TEST_SWEEP_CYCLES) + " cycles").c_str());
  robot.servo->sweep_full_range(TEST_SWEEP_CYCLES);
  
  // Return to center
  robot.servo->center();
  delay(1000);
}

void test_servo_speed_control() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Speed control");
  
  // Test slow speed
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Setting speed to 30°/s (slow)");
  robot.servo->set_speed(30);
  robot.servo->move_to_angle(0);
  robot.servo->move_to_angle(180);
  
  delay(500);
  
  // Test fast speed
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Setting speed to 120°/s (fast)");
  robot.servo->set_speed(120);
  robot.servo->move_to_angle(0);
  robot.servo->move_to_angle(180);
  
  // Restore default speed
  robot.servo->set_speed(DEFAULT_SERVO_SPEED);
  robot.servo->center();
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Restored default speed");
  
  delay(1000);
}

void run_all_servo_tests() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Starting all servo tests");
  
  test_servo_center();
  test_servo_move_to_angles();
  test_servo_move_to_limits();
  test_servo_smooth_movement();
  test_servo_sweep();
  test_servo_speed_control();
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "All servo tests complete");
}
