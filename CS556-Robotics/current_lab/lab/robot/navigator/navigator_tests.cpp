#include "navigator_tests.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "../robot.h"
#include "../utils/logger.h"

#undef CLASS_NAME
#define CLASS_NAME "NavigatorTests"

// External robot instance from lab3.ino
extern Robot robot;

// ========== HELPER FUNCTIONS ==========
static void print_odom_serial(float x, float y, float theta) {
  char msg[128];
  char xBuf[16];
  char yBuf[16];
  char thetaDegBuf[16];

  float thetaDeg = radians_to_degrees(theta);

  dtostrf(x, 0, 4, xBuf);
  dtostrf(y, 0, 4, yBuf);
  dtostrf(thetaDeg, 0, 2, thetaDegBuf);

  snprintf(msg, sizeof(msg), "odom: x=%s, y=%s, theta(deg)=%s", xBuf, yBuf, thetaDegBuf);
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg);
}

static void print_encoder_serial(int left, int right) {
  char msg[96];
  snprintf(msg, sizeof(msg), "encoders: left=%d, right=%d", left, right);
  Logger::log_info(CLASS_NAME, __FUNCTION__, msg);
}

void print_nav_odom_and_encoders() {

  int leftEnc = robot.navigator->getLeftEncoderCount();
  int rightEnc = robot.navigator->getRightEncoderCount();
  
  float x = robot.navigator->getX();
  float y = robot.navigator->getY();
  float theta = robot.navigator->getTheta();
  
  print_encoder_serial(leftEnc, rightEnc);
  
  print_odom_serial(x, y, theta);

  robot.display->print_odom_and_encoder(x, y, theta, leftEnc, rightEnc);

}

void print_nav_encoders() {

  int leftEnc = robot.navigator->getLeftEncoderCount();
  int rightEnc = robot.navigator->getRightEncoderCount();

  print_encoder_serial(leftEnc, rightEnc);

  robot.display->print_encoder(leftEnc, rightEnc);

}

void reset_nav_test_state() {
  robot.navigator->resetTotalEncoderCounts();
  robot.display->clear();
  print_nav_odom_and_encoders();
}

void drive_forward_with_updates(float distance_m, float speed_m_per_s, bool encoder_only = false) {
  const float segment_m = 0.1f;

  float remaining_m = distance_m;
  while (remaining_m > 0.0f) {
    float step_m = remaining_m > segment_m ? segment_m : remaining_m;
    robot.drive->move_forward(step_m, speed_m_per_s);
    robot.navigator->update();

    if (!encoder_only) {
      print_nav_odom_and_encoders();
    } else {
      print_nav_encoders();
    }

    remaining_m -= step_m;
  }
}

void drive_backward_with_updates(float distance_m, float speed_m_per_s, bool encoder_only = false) {
  const float segment_m = 0.1f;

  float remaining_m = distance_m;
  while (remaining_m > 0.0f) {
    float step_m = remaining_m > segment_m ? segment_m : remaining_m;
    robot.drive->move_backward(step_m, speed_m_per_s);
    robot.navigator->update();

    if (!encoder_only) {
      print_nav_odom_and_encoders();
    } else {
      print_nav_encoders();
    }

    remaining_m -= step_m;
  }
}

void rotate_with_updates(float theta_rad, float speed_m_per_s, bool encoder_only = false) {
  const float segment_rad = degrees_to_radians(15.0f);

  const bool turn_left = theta_rad >= 0.0f;
  float remaining_rad = turn_left ? theta_rad : -theta_rad;

  while (remaining_rad > 0.0f) {
    float step_rad = remaining_rad > segment_rad ? segment_rad : remaining_rad;

    if (turn_left) {
      robot.drive->turn_left(step_rad, speed_m_per_s, TurnMode::ANGLE);
    } else {
      robot.drive->turn_right(step_rad, speed_m_per_s, TurnMode::ANGLE);
    }

    robot.navigator->update();
    
    if (!encoder_only) {
      print_nav_odom_and_encoders();
    } else {
      print_nav_encoders();
    }

    remaining_rad -= step_rad;
  }
}

// ========== TEST TASKS ==========
void test_2_1_test_encoders_still() {
  delay(DELAY_SHORT_MS);
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.1: encoders while still");
  reset_nav_test_state();
  
  int left = (int)robot.navigator->getLeftEncoderCount();
  int right = (int)robot.navigator->getRightEncoderCount();

  print_encoder_serial(left, right);
  robot.display->print_encoder(left, right);
}

void test_2_2a_move_forward_1m() {
  delay(DELAY_LONG_MS);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.2a: move forward 1m");
  
  reset_nav_test_state();

  bool encoder_only = true;
  drive_forward_with_updates(ONE_METER_DIST, SAFE_SPEED_MPS, encoder_only);

  robot.drive->halt();

}

void test_2_2b_move_backward_1m() {
  delay(DELAY_LONG_MS);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.2b: move backward 1m");
  
  reset_nav_test_state();

  bool encoder_only = true;
  drive_backward_with_updates(ONE_METER_DIST, SAFE_SPEED_MPS, encoder_only);

  robot.drive->halt();
}

void test_2_2c_turn_left_90() {
  delay(DELAY_LONG_MS);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 2.2c: turn left 90 deg");
  
  reset_nav_test_state();

  bool encoder_only = true;
  rotate_with_updates(SQUARE_ANGLE_RAD, SAFE_SPEED_MPS, encoder_only);

  robot.drive->halt();
}

void test_3_2a_straight_line_15m() {
  delay(DELAY_LONG_MS);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 3.2a: straight line (configured dist)");
  
  reset_nav_test_state();
  
  bool encoder_only = false;
  drive_forward_with_updates(SQUARE_DIST_M, SQUARE_SPEED_MPS, encoder_only);
  
  robot.drive->halt();
}

void test_3_2b_square_clockwise() {
  delay(DELAY_LONG_MS);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 3.2b: square clockwise");
  
  reset_nav_test_state();
  
  bool encoder_only = false;

  for (int i = 0; i < 4; ++i) {
    drive_forward_with_updates(SQUARE_DIST_M, SQUARE_SPEED_MPS, encoder_only);
    rotate_with_updates(SQUARE_ANGLE_RAD, SQUARE_SPEED_MPS, encoder_only);
    delay(DELAY_SHORT_MS);
  }

  robot.drive->halt();
}

void test_3_2e_square_counter_clockwise() {
  delay(DELAY_LONG_MS);

  Logger::log_info(CLASS_NAME, __FUNCTION__, "Task 3.2e: square counterclockwise");

  reset_nav_test_state();

  bool encoder_only = false;

  for (int i = 0; i < 4; ++i) {
    drive_forward_with_updates(SQUARE_DIST_M, SQUARE_SPEED_MPS, encoder_only);
    rotate_with_updates(-SQUARE_ANGLE_RAD, SQUARE_SPEED_MPS, encoder_only);
    delay(DELAY_SHORT_MS);
  }

  robot.drive->halt();
}

void run_all_navigator_tests() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Starting all navigator tests");

  test_2_1_test_encoders_still();
  
  test_2_2a_move_forward_1m();
  test_2_2b_move_backward_1m();
  test_2_2c_turn_left_90();

  test_3_2a_straight_line_15m();
  test_3_2b_square_clockwise();
  test_3_2e_square_counter_clockwise();

  Logger::log_info(CLASS_NAME, __FUNCTION__, "All navigator tests complete");
}
