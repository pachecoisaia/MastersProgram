#include <Pololu3piPlus32U4.h>
#include "robot/robot.h"
#include "robot/utils/logger.h"
#include "robot/drivetrain/differential_drive_tests.h"

// Include implementation files so Arduino IDE compiles them
#include "robot/robot.cpp"
#include "robot/utils/logger.cpp"
#include "robot/utils/util.cpp"
#include "robot/drivetrain/differential_drive.cpp"
#include "robot/drivetrain/differential_drive_tests.cpp"

using namespace Pololu3piPlus32U4;

#undef CLASS_NAME
#define CLASS_NAME "lab1"

// Configuration constants
static const long BAUD_RATE = 9600;

// Initialize robot instance
Robot robot = Robot();

// ========== CONFIGURATION FUNCTIONS ==========

void validate_robot_initialization() {
  if (robot.drive == nullptr) {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Robot drive system not initialized!");
    while(1);  // Halt if initialization failed
  }
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Robot initialized successfully");
}

void setup() {
  Logger::configure(BAUD_RATE, LogLevel::INFO);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "=== Initializing Robot ===");
  validate_robot_initialization();
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "=== Configuring Robot Drivetrain ===");
  robot.drive->configure();
  Logger::log_info(CLASS_NAME, __FUNCTION__, "=== Configuration Complete ===");
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "=== Initializing Complete ===");

  print_test_parameters();
  
  delay(2000);  // Give time to read
}

void loop(){
    run_all_tests();
    
    robot.drive->halt();
    
    delay(5000);
    Logger::log_info(CLASS_NAME, __FUNCTION__, "=== Repeating tests ===");
}
