#include "robot.h"
#include "utils/logger.h"
#include "utils/util.h"

#undef CLASS_NAME
#define CLASS_NAME "Robot"

// ========== CONSTRUCTORS ==========

Robot::Robot() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Robot initialized with default configuration");

  drive = new DifferentialDrive();
  navigator = new Navigator();
  sonar = new Sonar();
  servo = new ServoController();
  display = new Display();

  Logger::log_info(CLASS_NAME, __FUNCTION__, "All subsystems initialized");
}
