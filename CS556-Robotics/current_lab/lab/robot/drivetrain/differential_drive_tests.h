#ifndef differential_drive_tests_h
#define differential_drive_tests_h

// Test parameters
const float ROBOT_SPEED_M_PER_S = 0.1;  // 0.1 m/s
const float DURATION_S = 0.94;          // 0.94 seconds
const float DISTANCE_M = 1.0;           // 1 meter

// Test functions for differential drive
void print_test_parameters();
void test_move_forward();
void test_move_backward();
void test_turn_right();
void test_turn_left();
void test_move_forward_turning_right();
void test_move_forward_turning_left();
void test_move_backward_turning_left();
void test_move_backward_turning_right();

// Run all tests in sequence
void run_all_tests();

#endif
