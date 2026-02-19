#ifndef navigator_tests_h
#define navigator_tests_h

#include "../utils/util.h"

const float SQUARE_DIST_M = 1.0f;                   // [m]
const float SQUARE_SPEED_MPS = 0.2f;                 // [m/s]
const float SQUARE_ANGLE_RAD = degrees_to_radians(90.0f); // [rad] = 90 deg
const float ONE_METER_DIST = 1.0f;                   // [m]
const float SAFE_SPEED_MPS = 0.2f;                   // [m/s]

const unsigned long DELAY_LONG_MS = 5000;      // [ms]
const unsigned long DELAY_SHORT_MS = 500;      // [ms]

void test_2_1_test_encoders_still();
void test_2_2a_move_forward_1m();
void test_2_2b_move_backward_1m();
void test_2_2c_turn_left_90();

void test_3_2a_straight_line_15m();
void test_3_2b_square_clockwise();
void test_3_2e_square_counter_clockwise();

void run_all_navigator_tests();

#endif
