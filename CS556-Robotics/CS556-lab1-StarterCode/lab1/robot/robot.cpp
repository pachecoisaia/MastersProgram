#include "robot.h"
#include "utils/logger.h"
#include "utils/util.h"

#undef CLASS_NAME
#define CLASS_NAME "Robot"

// ========== CONSTRUCTORS ==========

Robot::Robot() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Robot initialized with default configuration");
  
  // Initialize all subsystems
  drive = new DifferentialDrive();
  sonar = new Sonar();
  servo = new ServoController();
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "All subsystems initialized");
}
