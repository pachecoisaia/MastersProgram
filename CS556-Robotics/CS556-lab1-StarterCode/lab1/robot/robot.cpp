#include "robot.h"
#include "utils/logger.h"
#include "utils/util.h"

// ========== CONSTRUCTORS ==========

Robot::Robot() {
  Logger::log_info(__FUNCTION__, "Robot initialized with default configuration");
  drive = new DifferentialDrive();
}
