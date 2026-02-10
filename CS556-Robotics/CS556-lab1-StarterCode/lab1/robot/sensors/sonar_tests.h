#ifndef sonar_tests_h
#define sonar_tests_h

// Test parameters for sonar
const int TEST_MEASUREMENTS = 5;        // Number of measurements to take
const int TEST_DELAY_MS = 500;          // Delay between measurements

// Test functions for sonar sensor
void test_single_measurement();
void test_averaged_measurement();
void test_multiple_readings();
void test_configuration_changes();

// Run all sonar tests in sequence
void run_all_sonar_tests();

#endif
