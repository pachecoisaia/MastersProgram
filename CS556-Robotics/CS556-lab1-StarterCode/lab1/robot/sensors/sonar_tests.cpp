#include "sonar_tests.h"
#include "../robot.h"
#include "../utils/logger.h"
#include <Arduino.h>

#undef CLASS_NAME
#define CLASS_NAME "SonarTests"

// External robot instance from lab1.ino or lab2.ino
extern Robot robot;

void test_single_measurement() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Single distance measurement");
  
  float distance = robot.sonar->read_distance_cm();
  
  if (distance > 0) {
    String msg = "Distance: " + String(distance) + " cm";
    Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  } else {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Failed to read distance");
  }
  
  delay(1000);
}

void test_averaged_measurement() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Averaged distance measurement");
  
  float distance = robot.sonar->read_distance_averaged_cm();
  
  if (distance > 0) {
    String msg = "Averaged distance: " + String(distance) + " cm";
    Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
  } else {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Failed to read averaged distance");
  }
  
  delay(1000);
}

void test_multiple_readings() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Multiple consecutive readings");
  
  for (int i = 0; i < TEST_MEASUREMENTS; i++) {
    float distance = robot.sonar->read_distance_cm();
    String msg = "Reading " + String(i + 1) + ": " + String(distance) + " cm";
    Logger::log_info(CLASS_NAME, __FUNCTION__, msg.c_str());
    delay(TEST_DELAY_MS);
  }
}

void test_configuration_changes() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Test: Configuration changes");
  
  // Test with different sample counts
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Setting samples to 1");
  robot.sonar->set_num_samples(1);
  float dist1 = robot.sonar->read_distance_averaged_cm();
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Distance with 1 sample: " + String(dist1) + " cm").c_str());
  
  delay(500);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Setting samples to 5");
  robot.sonar->set_num_samples(5);
  float dist5 = robot.sonar->read_distance_averaged_cm();
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Distance with 5 samples: " + String(dist5) + " cm").c_str());
  
  // Restore default
  robot.sonar->set_num_samples(DEFAULT_NUM_SAMPLES);
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Restored default samples");
  
  delay(1000);
}

void run_all_sonar_tests() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Starting all sonar tests");
  
  test_single_measurement();
  test_averaged_measurement();
  test_multiple_readings();
  test_configuration_changes();
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "All sonar tests complete");
}
