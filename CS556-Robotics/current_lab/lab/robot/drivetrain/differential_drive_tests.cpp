#include "differential_drive_tests.h"
#include "../robot.h"
#include "../utils/logger.h"
#include <Arduino.h>

#undef CLASS_NAME
#define CLASS_NAME "DifferentialDriveTests"

// External robot instance from lab1.ino
extern Robot robot;

void print_test_parameters() {
  String test_params = "Test Parameters: Speed=" + String(ROBOT_SPEED_M_PER_S) + "m/s, Duration=" + String(DURATION_S) + "s, Distance=" + String(DISTANCE_M) + "m";
  Logger::log_info(CLASS_NAME, __FUNCTION__, test_params.c_str());
}

void test_move_forward() {
  String msg = "1. Moving forward " + String(DISTANCE_M) + " meter at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->move_forward(DISTANCE_M, ROBOT_SPEED_M_PER_S);
  delay(2000);
}

void test_move_backward() {
  String msg = "2. Moving backward " + String(DISTANCE_M) + " meter at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->move_backward(DISTANCE_M, ROBOT_SPEED_M_PER_S);
  delay(2000);
}

void test_turn_right() {
  String msg = "3. Turning right for " + String(DURATION_S) + " seconds at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->turn_right(DURATION_S, ROBOT_SPEED_M_PER_S, TurnMode::DURATION);
  delay(2000);
}

void test_turn_left() {
  String msg = "4. Turning left for " + String(DURATION_S) + " seconds at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->turn_left(DURATION_S, ROBOT_SPEED_M_PER_S, TurnMode::DURATION);
  delay(2000);
}

void test_move_forward_turning_right() {
  String msg = "5. Moving forward while turning right for " + String(DISTANCE_M) + " meter at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->move_forward_turning_right(DISTANCE_M, ROBOT_SPEED_M_PER_S);
  delay(2000);
}

void test_move_forward_turning_left() {
  String msg = "6. Moving forward while turning left for " + String(DISTANCE_M) + " meter at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->move_forward_turning_left(DISTANCE_M, ROBOT_SPEED_M_PER_S);
  delay(2000);
}

void test_move_backward_turning_left() {
  String msg = "7. Moving backward while turning left for " + String(DISTANCE_M) + " meter at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->move_backward_turning_left(DISTANCE_M, ROBOT_SPEED_M_PER_S);
  delay(2000);
}

void test_move_backward_turning_right() {
  String msg = "8. Moving backward while turning right for " + String(DISTANCE_M) + " meter at " + String(ROBOT_SPEED_M_PER_S) + " m/s";
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  robot.drive->move_backward_turning_right(DISTANCE_M, ROBOT_SPEED_M_PER_S);
  delay(2000);
}

void run_all_tests() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Starting all tests");
  test_move_forward();
  test_move_backward();
  test_turn_right();
  test_turn_left();
  test_move_forward_turning_right();
  test_move_forward_turning_left();
  test_move_backward_turning_left();
  test_move_backward_turning_right();
  Logger::log_info(CLASS_NAME, __FUNCTION__, "All tests complete");
}
