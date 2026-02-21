#include "navigator_tests.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../robot.h"
#include "../utils/logger.h"

#undef CLASS_NAME
#define CLASS_NAME "NavigatorTests"

// External robot instance from lab3.ino
extern Robot robot;

// ========== HELPER FUNCTIONS ==========
static void print_odom_serial(float x, float y, float theta) {
  constexpr size_t buf_sz = 128;
  char msg[buf_sz];
  char xBuf[16];
  char yBuf[16];
  char thetaDegBuf[16];

  float thetaDeg = radians_to_degrees(theta);

  dtostrf(x, 0, 4, xBuf);
  dtostrf(y, 0, 4, yBuf);
  dtostrf(thetaDeg, 0, 2, thetaDegBuf);

  snprintf(msg, buf_sz, "odom: x=%s, y=%s, theta(deg)=%s", xBuf, yBuf, thetaDegBuf);
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg);
}

static void print_encoder_serial(int64_t left, int64_t right) {
  // AVR printf does not reliably support %lld; downcast to long for logging.
  constexpr size_t buf_sz = 64;
  char msg[buf_sz];
  snprintf(msg, buf_sz, "encoders: left=%ld, right=%ld", (long)left, (long)right);
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg);
}

void print_nav_odom_and_encoders() {

  int64_t leftEnc = robot.navigator->getLeftEncoderCount();
  int64_t rightEnc = robot.navigator->getRightEncoderCount();
  
  float x = robot.navigator->getX();
  float y = robot.navigator->getY();
  float theta = robot.navigator->getTheta();
  
  print_encoder_serial(leftEnc, rightEnc);
  
  print_odom_serial(x, y, theta);

  robot.display->print_odom_and_encoder(x, y, theta, leftEnc, rightEnc);

}

void print_nav_encoders() {

  int64_t leftEnc = robot.navigator->getLeftEncoderCount();
  int64_t rightEnc = robot.navigator->getRightEncoderCount();

  print_encoder_serial(leftEnc, rightEnc);

  robot.display->print_encoder(leftEnc, rightEnc);

}

void drive_forward_with_updates(float distance_m, float speed_m_per_s, bool encoder_only = false) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "moving forward");

  robot.drive->move_forward(distance_m, speed_m_per_s);

  robot.navigator->update();

  if (!encoder_only) {
    print_nav_odom_and_encoders();
  } else {
    print_nav_encoders();
  }

  robot.drive->halt();
}

void drive_backward_with_updates(float distance_m, float speed_m_per_s, bool encoder_only = false) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "moving backward");
  
  robot.drive->move_backward(distance_m, speed_m_per_s);

  robot.navigator->update();

  if (!encoder_only) {
    print_nav_odom_and_encoders();
  } else {
    print_nav_encoders();
  }

  robot.drive->halt();
}

void turn_right_with_updates(float theta_rad, float speed_m_per_s, bool encoder_only = false) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "rotating right");
  
  robot.drive->turn_right(theta_rad, speed_m_per_s, TurnMode::ANGLE);

  robot.navigator->update();

  if (!encoder_only) {
    print_nav_odom_and_encoders();
  } else {
    print_nav_encoders();
  }

  robot.drive->halt();
}

void turn_left_with_updates(float theta_rad, float speed_m_per_s, bool encoder_only = false) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "rotating left");
  
  robot.drive->turn_left(theta_rad, speed_m_per_s, TurnMode::ANGLE);

  robot.navigator->update();

  if (!encoder_only) {
    print_nav_odom_and_encoders();
  } else {
    print_nav_encoders();
  }

  robot.drive->halt();
}


// ========== TEST TASKS ==========
void test_2_1_test_encoders_still() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.1: encoders while still");

  int left = (int)robot.navigator->getLeftEncoderCount();
  int right = (int)robot.navigator->getRightEncoderCount();

  print_encoder_serial(left, right);
  robot.display->print_encoder(left, right);
}

void test_2_2a_move_forward_1m() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.2a: move forward 1m");
  
  drive_forward_with_updates(1.0f, 0.2f, true);
}

void test_2_2b_move_backward_1m() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.2b: move backward 1m");
  
  drive_backward_with_updates(1.0f, 0.2f, true);
}

void test_2_2c_turn_left_90() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.2c: turn left 90 deg");
  float rad = degrees_to_radians(90.0f);
  turn_left_with_updates(rad, 0.2f, true);
}

void test_3_2a_straight_line_15m() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 3.2a: straight line (configured dist)");
  
  drive_forward_with_updates(15.0f, 0.2f, false);
}

void test_3_2b_square_clockwise() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 3.2b: square clockwise");
  
  for (int i = 0; i < 4; ++i) {
    drive_forward_with_updates(1.0f, 0.2f, false);
    float rad = degrees_to_radians(90.0f);
    turn_right_with_updates(rad, 0.2f, false);
  }

}

void test_3_2e_square_counter_clockwise() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 3.2e: square counterclockwise");

  for (int i = 0; i < 4; ++i) {
    drive_forward_with_updates(1.0f, 0.2f, false);
    float rad = degrees_to_radians(90.0f);
    turn_left_with_updates(rad, 0.2f, false);
  }
}
