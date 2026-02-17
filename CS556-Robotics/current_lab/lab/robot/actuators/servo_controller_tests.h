#ifndef servo_controller_tests_h
#define servo_controller_tests_h

// Test parameters for servo controller
const int TEST_ANGLE_1 = 45;            // Test angle 1
const int TEST_ANGLE_2 = 135;           // Test angle 2
const int TEST_SWEEP_CYCLES = 2;        // Number of sweep cycles

// Test functions for servo controller
void test_servo_center();
void test_servo_move_to_angles();
void test_servo_move_to_limits();
void test_servo_smooth_movement();
void test_servo_sweep();
void test_servo_speed_control();

// Run all servo tests in sequence
void run_all_servo_tests();

#endif
